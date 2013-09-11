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

#include "umcd/protocol/server/error_sender.hpp"
#include "umcd/protocol/make_header.hpp"
#include "umcd/protocol/server/close_on_error.hpp"
#include "umcd/special_packet.hpp"
#include "umcd/logger/asio_logger.hpp"
#include "config.hpp"

namespace umcd{

void async_send_error(const boost::shared_ptr<boost::asio::ip::tcp::socket> &socket, const std::string& error_message)
{
	UMCD_LOG_IP_FUNCTION_TRACER(socket);
	boost::shared_ptr<header_sender> sender = make_header_sender(socket, make_error_packet(error_message));
	sender->on_event<transfer_error>(boost::bind(&close_on_error, socket, _1));
	sender->async_send();
}

void async_send_error(const boost::shared_ptr<boost::asio::ip::tcp::socket> &socket, const boost::system::error_condition& error, const std::string& extra_msg)
{
	UMCD_LOG_IP_FUNCTION_TRACER(socket);
	async_send_error(socket, error.message() + extra_msg);
}
} // namespace umcd
