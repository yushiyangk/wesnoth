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

#ifndef UMCD_NETWORK_TRANSFER_HPP
#define UMCD_NETWORK_TRANSFER_HPP

#include "umcd/server/network_communicator.hpp"

template <class TransferOp, class BufferSequence>
class network_transfer : public network_communicator<BufferSequence>
{
public:
	typedef network_communicator<BufferSequence> base_type;
	typedef boost::asio::ip::tcp::socket socket_type;

	void async_transfer()
	{
		/** By default push to the back of the event handler queue, 
		* so it will be called after the buffer has been changed.
		* For this reason, don't put the on_event method in the constructor
		* (Here we are sure that the user won't add event).
		*/
		this->on_event(boost::bind(&network_transfer::async_transfer_chunk_event, this, _1, _2)
		  , event::chunk_complete
		);
		async_transfer_impl();
	}

protected:
	network_transfer(socket_type& socket, const BufferSequence& buffer)
	: base_type(buffer)
	, socket_(socket)
	{}

	network_transfer(socket_type& socket, const BufferSequence& buffer, std::size_t bytes_to_transfer)
	: base_type(buffer, bytes_to_transfer)
	, socket_(socket)
	{}

private:
	void async_transfer_impl()
	{
		if(!this->is_done())
		{
			static_cast<TransferOp*>(this)->async_transfer(socket_, this->buffer_);
		}
	}

	/** After a chunk has been received we want to continue receiving others (so we recall async_receive).
	*/
	void async_transfer_chunk_event(BufferSequence&, std::size_t&)
	{
		async_transfer_impl();
	}

	socket_type& socket_;
};

#endif // UMCD_NETWORK_SENDER_HPP
