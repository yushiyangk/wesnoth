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

#include "umcd/actions/basic_action.hpp"
#include "umcd/env/server_info.hpp"
#include "umcd/logger/asio_logger.hpp"
#include "umcd/protocol/server/error_sender.hpp"
#include "umcd/error.hpp"

#include "serialization/parser.hpp"
#include "wml_exception.hpp"
#include "filesystem.hpp"
#include "config.hpp"
#include <boost/current_function.hpp>

namespace umcd{

bool basic_action::validate(const socket_ptr& socket, const config& request, const std::string& validation_filename)
{
	try
	{
		server_info info;
		std::string validator_filename = info.wesnoth_dir() + get_umcd_protocol_schema_dir() + validation_filename;
		validator_type validator(validator_filename);
		validator.validate(request, "", 0, "");
		UMCD_LOG_IP(debug, socket) << BOOST_CURRENT_FUNCTION << " -- request validated.";
		return true;
	}
	catch(const twml_exception& e)
	{
		UMCD_LOG_IP(error, socket) << " -- invalid request at " << BOOST_CURRENT_FUNCTION << " (" << e.dev_message << ")";
		async_send_error(socket, make_error_condition(invalid_packet));
	}
	return false;
}

} // namespace umcd
