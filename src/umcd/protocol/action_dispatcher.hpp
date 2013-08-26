/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#ifndef UMCD_ACTION_DISPATCHER_HPP
#define UMCD_ACTION_DISPATCHER_HPP

#include "umcd/server/generic_factory.hpp"
#include "umcd/actions/basic_umcd_action.hpp"
#include "config.hpp"
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace umcd{

class action_dispatcher : public boost::enable_shared_from_this<action_dispatcher>
{
public:
	typedef boost::shared_ptr<basic_umcd_action> action_ptr;
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

private:
	typedef generic_factory<basic_umcd_action> action_factory_type;

public:
	static void init_action_factory();

	action_dispatcher(const socket_ptr& socket)
	: socket_(socket)
	{}

	void async_receive_request();

private:
	void dispatch();

	socket_ptr socket_;
	config header_metadata_;
	static action_factory_type action_factory_;
};

} // namespace umcd
#endif // UMCD_ACTION_DISPATCHER_HPP
