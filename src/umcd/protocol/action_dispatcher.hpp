/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

/** @file Dispatch incoming request on the good action. An action is a class
* that handles a specific request.
*/

#ifndef UMCD_ACTION_DISPATCHER_HPP
#define UMCD_ACTION_DISPATCHER_HPP

#include "umcd/server/generic_factory.hpp"
#include "umcd/actions/basic_action.hpp"
#include "config.hpp"
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace umcd{

class action_dispatcher : public boost::enable_shared_from_this<action_dispatcher>
{
public:
	typedef boost::shared_ptr<basic_action> action_ptr;
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

private:
	typedef generic_factory<basic_action> action_factory_type;

public:
	action_dispatcher(const socket_ptr& socket)
	: socket_(socket)
	{}

	/** Before starting to dispatch request, we must read it first.
	* We read it in this class because the data will only be used here.
	* The dispatch will be automatic after the reading.
	*/
	void async_receive_request();

private:
	/** Dispatch a successful read request on the good action.
	*/
	void dispatch();

	socket_ptr socket_;
	config header_metadata_;

	/** Boilerplate code that initialized the action factory.
	* This factory maps request name to action object.
	*/
	static action_factory_type action_factory;
 	struct action_factory_initializer
	{
		action_factory_initializer();
	};
	static action_factory_initializer action_init;
};

} // namespace umcd
#endif // UMCD_ACTION_DISPATCHER_HPP
