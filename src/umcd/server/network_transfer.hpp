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

/** @file Represents a transfer of data over the network.
*/

#ifndef UMCD_NETWORK_TRANSFER_HPP
#define UMCD_NETWORK_TRANSFER_HPP

#include "umcd/server/network_communicator.hpp"

/** Transfer some data with the help of the derived TransferOp class.
* It subscribes to the chunk_complete event to be sure to transfer all
* the data until it finishes.
*
* @see network_sender network_receiver
*/
template <class TransferOp, class BufferProvider>
class network_transfer : public network_communicator<network_transfer<TransferOp, BufferProvider>, BufferProvider>
{
public:
	typedef network_communicator<network_transfer<TransferOp, BufferProvider>, BufferProvider> base_type;
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

	/** Start an asynchronous transfer of data.
	*/
	void async_transfer()
	{
		assert(static_cast<bool>(socket));
		if(!this->is_done())
			async_transfer_impl();
	}

protected:
	network_transfer(const socket_ptr& socket, const boost::shared_ptr<BufferProvider>& buffer_provider)
	: base_type(buffer_provider)
	, socket_(socket)
	{}

	network_transfer(){}

public:
	/** Until the transfer is done, we relaunch the transfer operation.
	* @note Can't be private due to the CRTP..
	*/
	void async_transfer_impl()
	{
		static_cast<TransferOp*>(this)->async_transfer(socket_, this->use_buffer());
	}

private:
	socket_ptr socket_;
};

#endif // UMCD_NETWORK_SENDER_HPP
