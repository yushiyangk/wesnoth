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

#ifndef UMCD_CLIENT_CONNECTION_EVENT_HPP
#define UMCD_CLIENT_CONNECTION_EVENT_HPP

#include "umcd/server/events.hpp"
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <string>

namespace umcd{

struct try_connecting_with_ip{};
struct connection_success{};
struct connection_failure{};

} // umcd

template <>
struct event_slot<umcd::try_connecting_with_ip>
{
	/** Notify the fact that the client is trying to connect the server with a specific ip.
	*/
	typedef void type(const std::string&);
};

template <>
struct event_slot<umcd::connection_success>
{
	/** Notify the fact that the client is connected to the server with the ip passed in the arguments.
	*/
	typedef void type(const boost::shared_ptr<boost::asio::ip::tcp::socket>&);
};

template <>
struct event_slot<umcd::connection_failure>
{
	/** Notify the fact that an error occured and thus the client cannot connect to this server.
	*/
	typedef void type(const boost::system::error_code&);
};

namespace umcd{

struct client_connection_events : 
	events<boost::mpl::set<
					try_connecting_with_ip
				, connection_success
				, connection_failure> >
{};

} // namespace umcd
#endif // UMCD_CLIENT_CONNECTION_EVENT_HPP
