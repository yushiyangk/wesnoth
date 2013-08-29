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

#ifndef UMCD_CLIENT_HPP
#define UMCD_CLIENT_HPP

#include "umcd/client/client_connection_events.hpp"
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

namespace umcd{

class client : public boost::noncopyable
{
private:
	typedef boost::asio::ip::tcp::resolver resolver_type;
public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

	client(boost::asio::io_service &io_service)
	: io_service_(io_service)
	, socket_(boost::make_shared<socket_type>(boost::ref(io_service_)))
	, resolver_(io_service_)
	{}

	void async_connect(const std::string& host, const std::string& service)
	{
		// Start an asynchronous resolve to translate the server and service names
		// into a list of endpoints.
		boost::asio::ip::tcp::resolver::query query(host, service);
		resolver_.async_resolve(query,
			boost::bind(&client::handle_resolve, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::iterator));
	}

	template <class Event, class F>
	boost::signals2::connection on_event(F f)
	{
		return events_.on_event<Event>(f);
	}

	socket_ptr socket()
	{
		return socket_;
	}

private:
	std::string ip_address(const boost::asio::ip::tcp::endpoint &endpoint)
	{
		return endpoint.address().to_string()
				+	":"
				+ boost::lexical_cast<std::string>(endpoint.port());
	}

	resolver_type::iterator before_connect(
    const boost::system::error_code& error,
    resolver_type::iterator endpoint_iterator)
	{
		if(!error)
		{
			events_.signal_event<try_connecting_with_ip>(ip_address(*endpoint_iterator));
		}
		return endpoint_iterator;
	}

	void handle_resolve(const boost::system::error_code& error,
			resolver_type::iterator endpoint_iterator)
	{
		if(!error)
		{
			boost::asio::async_connect(*socket_
				, endpoint_iterator
				, boost::bind(&client::before_connect, this, _1, _2)
				, boost::bind(&client::handle_connect, this, _1, _2));
		}
		else
		{
			events_.signal_event<connection_failure>(error);
		}
	}

	void handle_connect(const boost::system::error_code& error,
			resolver_type::iterator endpoint_iterator)
	{
		if (!error)
		{
			events_.signal_event<connection_success>(ip_address(*endpoint_iterator));
		}
		else
		{
			events_.signal_event<connection_failure>(error);
		}
	}

	boost::asio::io_service &io_service_;
	socket_ptr socket_;
	resolver_type resolver_;
	client_connection_events events_;
};

} // namespace umcd

#endif
