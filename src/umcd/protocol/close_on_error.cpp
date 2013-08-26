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
#include "umcd/protocol/close_on_error.hpp"
#include "umcd/logger/asio_logger.hpp"
#include <boost/asio.hpp>
#include <cassert>

namespace umcd{
void close_on_error(const boost::shared_ptr<boost::asio::ip::tcp::socket> &socket, const boost::system::error_code& error)
{
	typedef boost::asio::ip::tcp::socket socket_type;
	assert(static_cast<bool>(error));
	UMCD_LOG_IP_FUNCTION_TRACER(socket);
	UMCD_LOG_IP(info, socket) << " -- unable to send data to the client (" << error.message() << "). Connection dropped.";
	socket->shutdown(socket_type::shutdown_both);
	socket->close();
}
} // namespace umcd
