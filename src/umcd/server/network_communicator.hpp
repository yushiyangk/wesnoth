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

#ifndef UMCD_NETWORK_COMMUNICATOR_HPP
#define UMCD_NETWORK_COMMUNICATOR_HPP

#include "umcd/server/transfer_events.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <cassert>

#include "umcd/boost/asio/asio.hpp"

template <class BufferProvider>
class network_communicator
	: private boost::noncopyable
	, public boost::enable_shared_from_this<network_communicator<BufferProvider> >
{
public:
	typedef typename BufferProvider::buffer_type buffer_type;

	std::size_t bytes_to_transfer() const;
	std::size_t bytes_transferred() const;
	bool is_done() const;
	buffer_type use_buffer() const;

	template <class Event, class F>
	boost::signals2::connection on_event(F f);

	// Should be protected but there is some problems in the derived class to access it in a bind declaration.
	std::size_t is_transfer_complete(const boost::system::error_code& error,
		std::size_t bytes_transferred);

	void on_chunk_complete(const boost::system::error_code& error,
		std::size_t bytes_transferred);

protected:
	/** The buffer size is equal to the number of bytes to transfer.
	*/
	network_communicator(const boost::shared_ptr<BufferProvider>& buffer_provider);

private:
	void update_bytes_transferred(std::size_t bytes_in_buffer);

	transfer_events events_;
	boost::shared_ptr<BufferProvider> buffer_provider_;
	std::size_t bytes_transferred_;
	std::size_t bytes_chunk_transferred_;
};

template <class BufferProvider>
network_communicator<BufferProvider>::network_communicator(const boost::shared_ptr<BufferProvider>& buffer_provider)
: buffer_provider_(buffer_provider)
, bytes_transferred_(0)
, bytes_chunk_transferred_(0)
{}

template <class BufferProvider>
std::size_t network_communicator<BufferProvider>::bytes_to_transfer() const
{
	return buffer_provider_->bytes_to_transfer();
}

template <class BufferProvider>
std::size_t network_communicator<BufferProvider>::bytes_transferred() const
{
	return bytes_transferred_;
}

template <class BufferProvider>
bool network_communicator<BufferProvider>::is_done() const
{
	return bytes_to_transfer() == bytes_transferred();
}

template <class BufferProvider>
typename network_communicator<BufferProvider>::buffer_type 
network_communicator<BufferProvider>::use_buffer() const
{
	return buffer_provider_->use_buffer();
}

template <class BufferProvider>
template <class Event, class F>
boost::signals2::connection network_communicator<BufferProvider>::on_event(F f)
{
	return events_.on_event<Event>(f);
}

template <class BufferProvider>
std::size_t network_communicator<BufferProvider>::is_transfer_complete(const boost::system::error_code& error,
	std::size_t bytes_in_buffer)
{
	update_bytes_transferred(bytes_in_buffer);
	if(error)
	{
		events_.signal_event<transfer_error>(error);
		return 0;
	}
	return bytes_to_transfer() - bytes_transferred();
}

template <class BufferProvider>
void network_communicator<BufferProvider>::on_chunk_complete(const boost::system::error_code& error,
	std::size_t bytes_in_buffer)
{
	update_bytes_transferred(bytes_in_buffer);
	if(error)
	{
		events_.signal_event<transfer_error>(error);
	}
	else
	{
		events_.signal_event<chunk_complete>(events_);
		if(is_done())
		{
			events_.signal_event<transfer_complete>();
		}
		else
		{
			// Prepare for the next chunk.
			bytes_chunk_transferred_ = 0;
		}
	}
}

template <class BufferProvider>
void network_communicator<BufferProvider>::update_bytes_transferred(std::size_t bytes_in_buffer)
{
	bytes_transferred_ += (bytes_in_buffer - bytes_chunk_transferred_);
	bytes_chunk_transferred_ = bytes_in_buffer;
}

#endif // UMCD_NETWORK_COMMUNICATOR_HPP
