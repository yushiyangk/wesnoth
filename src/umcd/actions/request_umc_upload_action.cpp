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
#include "addon_type.hpp"

namespace umcd{

const config& request_umc_upload_action::get_info(const config& metadata)
{
	return metadata.child("request_umc_upload").child("umc_configuration").child("info");
}

pod::addon_type request_umc_upload_action::retreive_addon_type_by_name(otl_connect& db, const std::string& addon_type_name)
{
	pod::addon_type addon_type;

	std::string select_addon_type_request = 
			"select * from addon_type where name = :addon_type_name<char[" 
		+ boost::lexical_cast<std::string>(addon_type_name.size()+1) 
		+ "]>";
	otl_stream select_addon_type(60
		, select_addon_type_request.c_str()
		, db);

	select_addon_type << addon_type_name;

	if(!select_addon_type.eof())
	{
		select_addon_type >> addon_type.value >> addon_type.name;
	}
	else
	{
		throw std::runtime_error("The field \"type\" in the upload request has not been checked "
														 "by the WML schema and thus cannot be found in the database.");
	}
	return addon_type;
}

void request_umc_upload_action::execute(const socket_ptr& socket, const config& request)
{
	UMCD_LOG_IP(trace, socket) << BOOST_CURRENT_FUNCTION;

	config upload_info = get_info(request);
	otl_connect &db = database().db();

	try
	{
		pod::addon_type addon_type = retreive_addon_type_by_name(db, upload_info["type"].str());
		std::cout << addon_type.name.data() << std::endl;
	}
	catch(const std::exception& e)
	{
		UMCD_LOG_IP(fatal, socket) << e.what();
		async_send_error(socket, make_error_condition(internal_error));
	}
}

request_umc_upload_action::~request_umc_upload_action(){}

} // namespace umcd
