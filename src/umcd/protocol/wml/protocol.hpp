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

#ifndef UMCD_PROTOCOL_HPP
#define UMCD_PROTOCOL_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/cstdint.hpp>
	 
#include "wml_exception.hpp"

#include "umcd/logger/asio_logger.hpp"
#include "umcd/protocol/error_sender.hpp"
#include "umcd/protocol/action_dispatcher.hpp"

namespace umcd{

class protocol : 
		public boost::enable_shared_from_this<protocol>
	,	private boost::noncopyable
{
public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;
	typedef boost::asio::io_service io_service_type;

public:
	// This constructor is only called once in main, so the factory will be created once as well.
	protocol(io_service_type& io_service);

	void handle_request();
	socket_type& socket();

private:
	socket_ptr socket_;
};

boost::shared_ptr<protocol> make_protocol(protocol::io_service_type& io_service);

} // namespace umcd

#endif // UMCD_PROTOCOL_HPP
