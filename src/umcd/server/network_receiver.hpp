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

#ifndef UMCD_NETWORK_RECEIVER_HPP
#define UMCD_NETWORK_RECEIVER_HPP

#include "umcd/server/network_transfer.hpp"

template <class MutableBufferSequence>
class network_receiver : public network_transfer<network_receiver<MutableBufferSequence>, MutableBufferSequence>
{
public:
	typedef MutableBufferSequence buffer_type;
	typedef network_transfer<network_receiver<buffer_type>, buffer_type> base_type;
	typedef boost::asio::ip::tcp::socket socket_type;

	void async_receive()
	{
		base_type::async_transfer();
	}

	network_receiver(socket_type& socket, const MutableBufferSequence& buffer)
	: base_type(socket, buffer)
	{}

	network_receiver(socket_type& socket, const MutableBufferSequence& buffer, std::size_t bytes_to_read)
	: base_type(socket, buffer, bytes_to_read)
	{}

	void async_transfer(socket_type& socket, const MutableBufferSequence& buffer)
	{
		boost::asio::async_read(socket
		, buffer
		, boost::bind(&base_type::is_transfer_complete, this->shared_from_this()
			, boost::asio::placeholders::error
			, boost::asio::placeholders::bytes_transferred)
		, boost::bind(&base_type::on_chunk_complete, this->shared_from_this()
			, boost::asio::placeholders::error
			, boost::asio::placeholders::bytes_transferred)
		);
	}
};

#endif // UMCD_NETWORK_RECEIVER_HPP
