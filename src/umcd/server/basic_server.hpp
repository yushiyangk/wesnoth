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

#ifndef SERVER_BASIC_SERVER_HPP
#define SERVER_BASIC_SERVER_HPP

#include "umcd/env/server_core.hpp"

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

class basic_server : boost::noncopyable
{
public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

public:
	explicit basic_server(const umcd::server_core& server_config, const boost::function<void(const socket_ptr&)> &request_handler);
	void run();
	boost::asio::io_service& io_service();

private:
	void start_accept();
	void handle_accept(const socket_ptr& socket, const boost::system::error_code& e);
	void handle_stop();

protected:
	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::acceptor acceptor_;
	boost::function<void(const socket_ptr&)> request_handler_;
	bool server_on_;
};

#endif // SERVER_BASIC_SERVER_HPP
