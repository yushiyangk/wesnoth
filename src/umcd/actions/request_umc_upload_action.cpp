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
#include "umcd/database.hpp"
#include "umcd/error.hpp"
#include "umcd/otl/otl.hpp"
#include "filesystem.hpp"
#include "config.hpp"
#include "umcd/pod/addon_type.hpp"
#include "umcd/pod/language.hpp"
#include "umcd/pod/addon.hpp"

namespace umcd{

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
	pod::addon_type addon_type;

	std::string select_addon_type_query = 
			"select * from addon_type where name = :addon_type_name<char[" 
		+ boost::lexical_cast<std::string>(addon_type_name.size()+1) 
		+ "]>";
	otl_stream select_addon_type(60
		, select_addon_type_query.c_str()
		, db);

	select_addon_type << addon_type_name;

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
	pod::language language;

	std::string select_language_query = 
			"select * from language where name = :language_name<char[" 
		+ boost::lexical_cast<std::string>(language_name.size()+1) 
		+ "]>";
	otl_stream select_language(60
		, select_language_query.c_str()
		, db);

	select_language << language_name;

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
	boost::optional<pod::addon> addon;

	std::string select_addon_query = "select * from addon where id = :id<unsigned>";
	otl_stream select_addon(60
		, select_addon_query.c_str()
		, db);

	select_addon << id;

	if(!select_addon.eof())
	{
		addon = pod::addon();
		select_addon >> addon->id >> addon->type >> addon->email >> addon->password >> addon->native_language;
	}
	return addon;
}

void request_umc_upload_action::execute(const socket_ptr& socket, const config& request)
{
	UMCD_LOG_IP(trace, socket) << BOOST_CURRENT_FUNCTION;

	config upload_info = get_info(request);
	config upload_lang = get_lang(request);
	otl_connect &db = database().db();

	try
	{
		// The ID is in the request so we guess the user want to update a UMC.
		if(upload_info.has_attribute("id"))
		{
			boost::optional<pod::addon> addon = retreive_addon_by_id(db, upload_info["id"].to_unsigned());
			if(!addon)
			{
				UMCD_LOG_IP(debug, socket) << "User trying to update an unknown UMC (search by id).";
				async_send_error(socket, make_error_condition(bad_umc_id));
			}
		}
		// The ID is not in the request. It's a new UMC.
		else
		{
			pod::addon addon;
			addon.type = retreive_addon_type_by_name(db, upload_info["type"].str()).value;
			addon.native_language = retreive_language_by_name(db, upload_lang["native_language"].str()).value;
			std::string email = upload_info["email"].str();
			std::string password = upload_info["password"].str();
			if(email.size() > addon.email.size())
				async_send_error(socket, make_error_condition(field_too_long), "email");
			else if(password.size() > addon.password.size())
				async_send_error(socket, make_error_condition(field_too_long), "password");
			else
			{
				//create_addon()
			}
		}
	}
	catch(const std::exception& e)
	{
		UMCD_LOG_IP(fatal, socket) << e.what();
		async_send_error(socket, make_error_condition(internal_error));
	}
}

request_umc_upload_action::~request_umc_upload_action(){}

} // namespace umcd
