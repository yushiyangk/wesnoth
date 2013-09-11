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

#ifndef UMCD_ERROR_SENDER_HPP
#define UMCD_ERROR_SENDER_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

namespace umcd{

/** Asynchronously send an error and close the socket if it fails to send it.
*/
void async_send_error(const boost::shared_ptr<boost::asio::ip::tcp::socket> &socket, const std::string& error_message);
void async_send_error(const boost::shared_ptr<boost::asio::ip::tcp::socket> &socket, const boost::system::error_condition& error, const std::string& extra_msg = "");
} // namespace umcd

#endif // UMCD_ERROR_SENDER_HPP
