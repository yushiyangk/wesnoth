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

#ifndef UMCD_NETWORK_SENDER_HPP
#define UMCD_NETWORK_SENDER_HPP

#include "umcd/server/network_transfer.hpp"

template <class ConstBufferSequence>
class network_sender : public network_transfer<network_sender<ConstBufferSequence>, ConstBufferSequence>
{
public:
	typedef ConstBufferSequence buffer_type;
	typedef network_transfer<network_sender<buffer_type>, buffer_type> base_type;
	typedef boost::asio::ip::tcp::socket socket_type;

	void async_send()
	{
		base_type::async_transfer();
	}

	network_sender(socket_type& socket, const ConstBufferSequence& buffer)
	: base_type(socket, buffer)
	{}

	network_sender(socket_type& socket, const ConstBufferSequence& buffer, std::size_t bytes_to_write)
	: base_type(socket, buffer, bytes_to_write)
	{}

	void async_transfer(socket_type& socket, const ConstBufferSequence& buffer)
	{
		boost::asio::async_write(socket
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

#endif // UMCD_NETWORK_SENDER_HPP
