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

#ifndef UMCD_SERVER_BASIC_SERVER_HPP
#define UMCD_SERVER_BASIC_SERVER_HPP

#include "umcd/server/server_events.hpp"
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <string>

class basic_server : boost::noncopyable
{
public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

public:
	/** Initialize the server.
	* @post The server is not launched.
	*/
	basic_server();

	/** We asynchronously start the server by trying to connect the specified service.
	* @note Nothing is done until run is called.
	*/
	void start(const std::string& service);

	/** The main boucle of the server is launched.
	* @note run doesn't return.
	*/
	void run();

	/**
	* @return the io_service associated with this server.
	*/
	boost::asio::io_service& get_io_service();

	/** Add an event to the current object.
  * @pre Event must be an event of the server_events class.
	*/
	template <class Event, class F>
	boost::signals2::connection on_event(F f)
	{
		return events_.on_event<Event>(f);
	}

private:
	void start_accept();
	void handle_accept(const socket_ptr& socket, const boost::system::error_code& e);
	void handle_stop();

	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::acceptor acceptor_;
	bool server_on_;
	server_events events_;
};

#endif // UMCD_SERVER_BASIC_SERVER_HPP
