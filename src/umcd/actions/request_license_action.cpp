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

/* Design rational:
The license is not shipped with the Wesnoth client because this server can be re-use with different licenses on other server than Wesnoth ones.
*/

#include "umcd/actions/request_license_action.hpp"
#include "umcd/protocol/server/error_sender.hpp"
#include "umcd/protocol/make_header.hpp"
#include "umcd/protocol/server/close_on_error.hpp"
#include "umcd/error.hpp"
#include "umcd/env/server_info.hpp"
#include "umcd/logger/asio_logger.hpp"
#include "filesystem.hpp"
#include "config.hpp"
#include "serialization/parser.hpp"
#include "wml_exception.hpp"
#include <boost/current_function.hpp>

namespace umcd{

void request_license_action::execute(const socket_ptr& socket, const config& request)
{
	try
	{
		// (1) Validation of the request.
		server_info info;
		std::string validator_filename = info.wesnoth_dir() + get_umcd_protocol_schema_dir() + "/request_license.cfg";
		validator_type validator(validator_filename);
		config dummy;
		::read(dummy, request.to_string(), &validator);
		UMCD_LOG_IP(debug, socket) << BOOST_CURRENT_FUNCTION << " -- request validated.";

		// (2) Creation of the reply.
		// NOTE: We don't use the COPYING file because the " are not double quoted, instead we use a preformatted license file with " replaced by "".
		config reply("request_license");
		reply.child("request_license")["text"] = "\"" + read_file(info.wesnoth_dir() + get_umcd_license_file()) + "\"";

		// (3) Sending the reply.
		boost::shared_ptr<core::header_const_buffer::sender_type> sender = make_header_sender(socket, reply);
		sender->on_event<transfer_error>(boost::bind(&close_on_error, socket, _1));
		sender->async_send();
	}
	catch(const twml_exception& e)
	{
		UMCD_LOG_IP(error, socket) << " -- invalid request at " << BOOST_CURRENT_FUNCTION << " (" << e.dev_message << ")";
		async_send_error(socket, make_error_condition(invalid_packet));
	}
}

request_license_action::~request_license_action(){}

} // namespace umcd
