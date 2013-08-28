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

#ifndef UMCD_CLOSE_ON_ERROR_HPP
#define UMCD_CLOSE_ON_ERROR_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

namespace umcd{
/** Log and close the socket on error. This is designed to be an event handler so an error must have occurred.
*/
void close_on_error(const boost::shared_ptr<boost::asio::ip::tcp::socket> &socket, const boost::system::error_code& error);
} // namespace umcd

#endif // UMCD_CLOSE_ON_ERROR_HPP
