/*
	Copyright (C) 2013 by Pierre Talbot <ptalbot@hyc.io>
	Part of the Battle for Wesnoth Project http://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

/** @file Asynchronously receive data from the network.
*/

#ifndef UMCD_NETWORK_RECEIVER_HPP
#define UMCD_NETWORK_RECEIVER_HPP

#include "umcd/server/network_transfer.hpp"

/** Wrap calls the boost::asio::async_read method and bind argument to it.
*
* @see network_sender network_transfer
*/
template <class BufferProvider>
class network_receiver : public network_transfer<network_receiver<BufferProvider>, BufferProvider>
{
public:
	typedef typename BufferProvider::buffer_type buffer_type;
	typedef network_transfer<network_receiver<BufferProvider>, BufferProvider> base_type;
	typedef typename base_type::socket_ptr socket_ptr;

	/** Asynchronously receive data from the socket and put them in the associated buffer provider.
	* @note This class must stay alive until the event transfer_complete or transfer_error has been launched.
	*/
	void async_receive()
	{
		base_type::async_transfer();
	}

	void async_receive(const boost::posix_time::time_duration &timeout)
	{
		base_type::async_transfer(timeout);
	}


	/**
	* @param socket Receive data from this socket.
	* @param buffer_provider Provide buffer to receive data in.
	*/
	network_receiver(const socket_ptr& socket, const boost::shared_ptr<BufferProvider>& buffer_provider)
	: base_type(socket, buffer_provider)
	{}

	void async_transfer(const socket_ptr& socket, const buffer_type& buffer)
	{
		boost::asio::async_read(*socket
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
