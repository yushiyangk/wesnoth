/*
	Copyright (C) 2013 by Pierre Talbot <ptalbot@mopong.net>
	Part of the Battle for Wesnoth Project http://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#include "umcd/actions/request_umc_upload_action.hpp"
#include "umcd/env/server_info.hpp"
#include "umcd/protocol/server/error_sender.hpp"
#include "umcd/logger/asio_logger.hpp"
#include "umcd/database/database.hpp"
#include "umcd/database/database_query.hpp"
#include "umcd/database/connection_instance.hpp"
#include "umcd/error.hpp"
#include "umcd/otl/otl.hpp"
#include "filesystem.hpp"
#include "config.hpp"
#include "umcd/pod/addon_type.hpp"
#include "umcd/pod/language.hpp"
#include "umcd/pod/addon.hpp"

namespace umcd{

static std::string char_array_str(const std::string& data)
{
	return "char[" + boost::lexical_cast<std::string>(data.size()+1) + "]";
}

const config& request_umc_upload_action::get_info(const config& metadata)
{
	return metadata.child("request_umc_upload").child("umc_configuration").child("info");
}

const config& request_umc_upload_action::get_lang(const config& metadata)
{
	return metadata.child("request_umc_upload").child("umc_configuration").child("language");
}

pod::addon_type request_umc_upload_action::retreive_addon_type_by_name(otl_connect& db, const std::string& addon_type_name)
{
	UMCD_LOG_FUNCTION_TRACER();
	pod::addon_type addon_type;

	std::string select_addon_type_query = 
				"select * from addon_type where name = :addon_type_name<"
			+ char_array_str(addon_type_name) + ">";

	UMCD_LOG(debug) << select_addon_type_query;

	otl_stream select_addon_type(60
		, select_addon_type_query.c_str()
		, db);

	select_addon_type << addon_type_name.c_str();

	if(!select_addon_type.eof())
	{
		select_addon_type >> addon_type.value >> addon_type.name;
	}
	else
	{
		throw std::runtime_error("The field \"type\" in table \"addon_type\" (" + addon_type_name + ") in the upload request has not "
														 "been checked by the WML schema and thus cannot be found in the database.");
	}
	return addon_type;
}

pod::language request_umc_upload_action::retreive_language_by_name(otl_connect& db, const std::string& language_name)
{
	UMCD_LOG_FUNCTION_TRACER();
	pod::language language;

	std::string select_language_query = 
			"select * from language where name = :language_name<"
		+ char_array_str(language_name) + ">";

	UMCD_LOG(debug) << select_language_query;

	otl_stream select_language(60
		, select_language_query.c_str()
		, db);

	select_language << language_name.c_str();

	if(!select_language.eof())
	{
		select_language >> language.value >> language.name;
	}
	else
	{
		throw std::runtime_error("The field \"type\" in table \"language\" (" + language_name + ") in the upload "
														 "request has not been checked by the WML schema and thus cannot be found in the database.");
	}
	return language;
}

boost::optional<pod::addon> request_umc_upload_action::retreive_addon_by_id(otl_connect& db, boost::uint32_t id)
{
	UMCD_LOG_FUNCTION_TRACER();
	boost::optional<pod::addon> addon;

	std::string select_addon_query = "select * from addon where id = :id<unsigned>";
	otl_stream select_addon(60
		, select_addon_query.c_str()
		, db);

	UMCD_LOG(debug) << select_addon_query;

	select_addon << id;

	if(!select_addon.eof())
	{
		addon = pod::addon();
		select_addon >> addon->id >> addon->type >> addon->email >> addon->password >> addon->native_language;
	}
	return addon;
}

void request_umc_upload_action::update_umc(const boost::shared_ptr<connection_instance>& db_connection, const socket_ptr& socket, const config& request)
{
	UMCD_LOG_IP_FUNCTION_TRACER(socket);
	const config& upload_info = get_info(request);
	otl_connect& db = db_connection->get();
	try
	{
		boost::optional<pod::addon> addon = retreive_addon_by_id(db, upload_info["id"].to_unsigned());
		if(!addon)
		{
			UMCD_LOG_IP(debug, socket) << "User trying to update an unknown UMC (search by id).";
			async_send_error(socket, make_error_condition(bad_umc_id));
		}
	}
	catch(const std::exception& e)
	{
		UMCD_LOG_IP(fatal, socket) << e.what();
		async_send_error(socket, make_error_condition(internal_error));
	}
}

void request_umc_upload_action::add_addon(otl_connect& db, pod::addon& addon)
{
	UMCD_LOG_FUNCTION_TRACER();
	std::string insert_addon_query = "insert into addon (type, email, password, native_language) values("
		":type<unsigned>,"
		":email<" + char_array_str(addon.email) + ">,"
		":password<" + char_array_str(addon.password) + ">,"
		":native_language<unsigned>)";

	UMCD_LOG(debug) << insert_addon_query;

	otl_stream insert_addon(100
		, insert_addon_query.c_str()
		, db);

	insert_addon << addon.type << addon.email << addon.password << addon.native_language;

	// Flush to retreive the last inserted ID.
	insert_addon.flush();

	otl_stream last_inserted_id(50,
			database_query::get<query::last_inserted_id>().c_str()
		, db);

	last_inserted_id >> addon.id;

	UMCD_LOG(debug) << "Last inserted ID: " << addon.id;
}

void request_umc_upload_action::create_umc(const boost::shared_ptr<connection_instance>& db_connection, const socket_ptr& socket, const config& request)
{
	UMCD_LOG_IP_FUNCTION_TRACER(socket);
	const config& upload_info = get_info(request);
	const config& upload_lang = get_lang(request);

	otl_connect& db = db_connection->get();
	try
	{
		pod::addon addon;
		addon.native_language = retreive_language_by_name(db, upload_lang["native_language"].str()).value;
		addon.type = retreive_addon_type_by_name(db, upload_info["type"].str()).value;
		addon.email = upload_info["email"].str(); 
		addon.password = upload_info["password"].str();
		add_addon(db, addon);
	}
	catch(const std::exception& e)
	{
		UMCD_LOG_IP(fatal, socket) << e.what();
		async_send_error(socket, make_error_condition(internal_error));
	}
}

void request_umc_upload_action::on_db_timeout(const socket_ptr& socket)
{
	UMCD_LOG_IP(warning, socket) << "The database timed out before handling the query in a reasonnable time.";
	async_send_error(socket, make_error_condition(internal_error));
}

void request_umc_upload_action::execute(const socket_ptr& socket, const config& request)
{
	UMCD_LOG_IP(trace, socket) << BOOST_CURRENT_FUNCTION;

	if(validate(socket, request, "/request_umc_upload.cfg"))
	{
		const config& upload_info = get_info(request);

		// The ID is in the request so we guess the user want to update a UMC.
		if(upload_info.has_attribute("id"))
		{
			dispatch_query(socket->get_io_service()
				, boost::bind(&request_umc_upload_action::update_umc, shared_from_this(), _1, socket, request)
				, boost::bind(&request_umc_upload_action::on_db_timeout, shared_from_this(), socket)
			);
		}
		// The ID is not in the request. It's a new UMC.
		else
		{
			dispatch_query(socket->get_io_service()
				, boost::bind(&request_umc_upload_action::create_umc, shared_from_this(), _1, socket, request)
				, boost::bind(&request_umc_upload_action::on_db_timeout, shared_from_this(), socket)
			);
		}
	}
}

request_umc_upload_action::~request_umc_upload_action(){}

} // namespace umcd
