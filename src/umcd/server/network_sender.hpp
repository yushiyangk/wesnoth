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

#include "umcd/server/network_communicator.hpp"

template <class ConstBufferSequence>
class network_sender : public network_communicator<ConstBufferSequence>
{
public:
	typedef network_communicator<ConstBufferSequence> base_type;
	typedef boost::asio::ip::tcp::socket socket_type;

	void async_send()
	{
		/** By default push to the back of the event handler queue, 
		* so it will be called after the buffer has been changed.
		* For this reason, don't put the on_event method in the constructor
		* (Here we are sure that the user won't add event).
		*/
		this->on_event(boost::bind(&network_sender::async_send_chunk_event, this, _1)
		  , event::chunk_complete
		);
		async_send_impl();
	}

protected:
	network_sender(socket_type& socket, const ConstBufferSequence& buffer)
	: base_type(buffer)
	, socket_(socket)
	{}

	network_sender(socket_type& socket, const ConstBufferSequence& buffer, std::size_t bytes_to_write)
	: base_type(buffer, bytes_to_write)
	, socket_(socket)
	{}

private:
	void async_send_impl()
	{
		if(!this->is_done())
		{
			boost::asio::async_write(socket_
			, this->buffer_
			, boost::bind(&base_type::is_transfer_complete, this->shared_from_this()
				, boost::asio::placeholders::error
				, boost::asio::placeholders::bytes_transferred)
			, boost::bind(&base_type::chunk_complete, this->shared_from_this()
				, boost::asio::placeholders::error
				, boost::asio::placeholders::bytes_transferred)
			);
		}
	}

	/** After a chunk has been sent we want to continue sending others (so we recall async_send).
	*/
	void async_send_chunk_event(ConstBufferSequence&)
	{
		async_send_impl();
	}

	socket_type& socket_;
};

#endif // UMCD_NETWORK_SENDER_HPP
