/*
	Copyright (C) 2013 by Pierre Talbot <ptalbot@hyc.io>
	Part of the Battle for Wesnoth Project http://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#include "umcd/protocol/server/entry_point.hpp"
#include "umcd/protocol/server/action_dispatcher.hpp"
#include "umcd/logger/asio_logger.hpp"

namespace umcd{
void protocol_entry_point(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket)
{
	UMCD_LOG_IP_FUNCTION_TRACER(socket);
	boost::shared_ptr<action_dispatcher> dispatcher = boost::make_shared<action_dispatcher>(socket);
	dispatcher->async_receive_request();
}
} // namespace umcd
