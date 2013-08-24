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

template <class BufferProvider>
class network_sender : public network_transfer<network_sender<BufferProvider>, BufferProvider>
{
public:
	typedef typename BufferProvider::buffer_type buffer_type;
	typedef network_transfer<network_sender<BufferProvider>, BufferProvider> base_type;
	typedef typename base_type::socket_ptr socket_ptr;

	void async_send()
	{
		base_type::async_transfer();
	}

	network_sender(const socket_ptr& socket, const boost::shared_ptr<BufferProvider>& buffer_provider)
	: base_type(socket, buffer_provider)
	{}

	void async_transfer(const socket_ptr& socket, const buffer_type& buffer)
	{
		boost::asio::async_write(*socket
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
