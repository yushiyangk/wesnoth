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
#include "umcd/protocol/error_sender.hpp"
#include "umcd/error.hpp"
#include "umcd/env/server_info.hpp"
#include "umcd/logger/asio_logger.hpp"
#include "filesystem.hpp"
#include "config.hpp"
#include "serialization/parser.hpp"
#include "wml_exception.hpp"
#include <boost/current_function.hpp>

namespace umcd{

request_license_action::request_license_action(const socket_ptr& socket)
: socket_(socket)
{}

request_license_action::request_license_action(){}

void request_license_action::execute(const config& request)
{
	try
	{
		server_info info;
		std::string validator_filename = info.wesnoth_dir() + get_umcd_protocol_schema_dir() + "/request_license.cfg";
		validator_type validator(validator_filename);
		config dummy;
		::read(dummy, request.to_string(), &validator);
		UMCD_LOG_IP(debug, *socket_) << BOOST_CURRENT_FUNCTION << " -- request validated.";

		// NOTE: We don't use the COPYING file because the " are not double quoted, instead we use a preformatted license file with " replaced by "".
		config reply("request_license");
		reply.child("request_license")["text"] = "\"" + read_file(info.wesnoth_dir() + get_umcd_license_file()) + "\"";
	}
	catch(const twml_exception& e)
	{
		UMCD_LOG_IP(error, *socket_) << " -- invalid request at " << BOOST_CURRENT_FUNCTION << " (" << e.dev_message << ")";
		async_send_error(socket_, make_error_condition(invalid_packet));
	}
}

boost::shared_ptr<request_license_action::base_type> request_license_action::clone() const
{
	return boost::shared_ptr<base_type>(new request_license_action(*this));
}

} // namespace umcd
