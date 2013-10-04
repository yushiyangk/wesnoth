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

#ifndef UMCD_ENTRY_POINT_HPP
#define UMCD_ENTRY_POINT_HPP

#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

namespace umcd{
/** This is the protocol entry point after the server has just accepted a client.
*/
void protocol_entry_point(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket);
} // namespace umcd

#endif // UMCD_ENTRY_POINT_HPP
