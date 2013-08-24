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

#include <boost/lexical_cast.hpp>
#include <boost/current_function.hpp>
#include <boost/assert.hpp>

#include "umcd/protocol/wml/protocol.hpp"
#include "umcd/special_packet.hpp"
#include "umcd/error.hpp"
#include "umcd/env/server_info.hpp"
#include "umcd/env/protocol_info.hpp"

#include "umcd/protocol/header_data.hpp"

#define FUNCTION_TRACER() UMCD_LOG_IP_FUNCTION_TRACER(*socket_)

namespace umcd{


protocol::protocol(io_service_type& io_service)
: socket_(boost::make_shared<socket_type>(boost::ref(io_service)))
{
}

protocol::socket_type& protocol::socket()
{
	return *socket_;
}

void protocol::handle_request()
{
	FUNCTION_TRACER();

	boost::shared_ptr<action_dispatcher> dispatcher = boost::make_shared<action_dispatcher>(socket_);
	dispatcher->async_receive_request();
}

boost::shared_ptr<protocol> make_protocol(protocol::io_service_type& io_service)
{
	return boost::make_shared<protocol>(boost::ref(io_service));
}

} // namespace umcd
