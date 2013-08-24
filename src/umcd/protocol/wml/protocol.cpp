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

#include <boost/lexical_cast.hpp>
#include <boost/current_function.hpp>
#include <boost/assert.hpp>

#include "umcd/protocol/wml/protocol.hpp"
#include "umcd/special_packet.hpp"
#include "umcd/error.hpp"
#include "umcd/env/server_info.hpp"
#include "umcd/env/protocol_info.hpp"

#include "umcd/protocol/header_data.hpp"
#include "umcd/protocol/action_dispatcher.hpp"

namespace umcd{

std::size_t protocol::REQUEST_HEADER_MAX_SIZE = 8192;

#define FUNCTION_TRACER() UMCD_LOG_IP_FUNCTION_TRACER(*socket_)

protocol::protocol(io_service_type& io_service, const environment& env)
: environment_(env)
, socket_(boost::make_shared<socket_type>(boost::ref(io_service)))
{
}

config& protocol::get_reply()
{
	return reply_;
}

void protocol::load(const protocol_info& proto_info)
{
	REQUEST_HEADER_MAX_SIZE = proto_info.header_max_size();
}

protocol::socket_type& protocol::socket()
{
	return *socket_;
}

void protocol::complete_request(const boost::system::error_code& error, std::size_t)
{
	FUNCTION_TRACER();
	if(error)
	{
		UMCD_LOG_IP(info, *socket_) << " -- unable to send data to the client (" << error.message() << "). Connection dropped.";
		// TODO close socket.
	}
}

void protocol::on_error(const boost::system::error_code& error)
{
	FUNCTION_TRACER();
	if(error)
	{
		UMCD_LOG_IP(info, *socket_) << " -- unable to send data to the client (" << error.message() << "). Connection dropped.";
		// TODO close socket.
	}
}

void protocol::async_send_reply()
{
	FUNCTION_TRACER();

	boost::shared_ptr<header_const_buffer::sender_type> sender = make_header_sender(socket_, reply_);
	sender->on_event<transfer_error>(boost::bind(&protocol::on_error, shared_from_this(), boost::asio::placeholders::error));
	sender->async_send();
}

void protocol::async_send_invalid_packet(const std::string &where, const std::exception& e)
{
	UMCD_LOG_IP(error, *socket_) << " -- invalid request at " << where << " (" << e.what() << ")";
	async_send_error(socket_, make_error_condition(invalid_packet));
}

void protocol::async_send_invalid_packet(const std::string &where, const twml_exception& e)
{
	UMCD_LOG_IP(error, *socket_) << " -- invalid request at " << where 
										<< " (user message=" << e.user_message << " ; dev message=" << e.dev_message << ")";
	async_send_error(socket_, make_error_condition(invalid_packet));
}

void protocol::handle_request()
{
	FUNCTION_TRACER();

	boost::shared_ptr<action_dispatcher> dispatcher = boost::make_shared<action_dispatcher>(socket_);
	dispatcher->async_receive_request();
}

boost::shared_ptr<protocol> make_protocol(protocol::io_service_type& io_service, const environment& env)
{
	return boost::make_shared<protocol>(boost::ref(io_service), boost::cref(env));
}

} // namespace umcd
