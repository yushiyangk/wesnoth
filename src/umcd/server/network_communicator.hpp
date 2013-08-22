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

#include "umcd/server/events.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <cassert>

#include "umcd/boost/asio/asio.hpp"

struct transfer_complete{};
struct transfer_error{};
struct transfer_on_going{}; // Useful for refresh.
struct chunk_complete{}; // Useful to launch the next op.

template <>
struct event_slot<transfer_complete>
{
	typedef void type();
};

template <>
struct event_slot<transfer_error>
{
	typedef void type(const boost::system::error_code&);
};

template <>
struct event_slot<transfer_on_going>
{
	typedef void type(std::size_t, std::size_t);
};

template <>
struct event_slot<chunk_complete>
{
	typedef void type(std::size_t&);
};

template <class BufferSequence>
class network_communicator
	: private boost::noncopyable
	, public boost::enable_shared_from_this<network_communicator<BufferSequence> >
{
public:
	typedef BufferSequence buffer_type;

	std::size_t bytes_to_transfer() const;
	std::size_t bytes_transferred() const;
	bool is_done() const;

	typedef events<boost::mpl::set<
				transfer_complete
			, transfer_error
			, transfer_on_going
			, chunk_complete> > events_type;

	template <class Event, class F>
	boost::signals2::connection on_event(F f);

	/**
	@param slot_function A function with no argument, just to notify the fact that the transfer is finished.
	*/
	//boost::signals2::connection on_event(boost::function<void()> slot_function, event::transfer_complete_tag);

	/**
	@param slot_function A function that takes an error_code and is called if the transmission fails.
	*/
	//boost::signals2::connection on_event(boost::function<void (const boost::system::error_code&)> slot_function, event::transfer_error_tag);

	/** Use this event to track the transmission process.
	@param slot_function A function that takes the bytes transferred and the bytes to transfer (total).
	*/
	//boost::signals2::connection on_event(boost::function<void (std::size_t, std::size_t)> slot_function, event::transfer_on_going_tag);

	/** Use this event to dynamically change the buffer and the bytes to transfer or simply to track the chunk process.
	The last can be useful to relaunch a transmission operation if the transmission is not finished yet.
	@param slot_function A function that takes references to the buffer and the bytes to transfer.
	*/
	//boost::signals2::connection on_event(boost::function<void (buffer_type&, std::size_t&)> slot_function, event::chunk_complete_tag);

	// Should be protected but there is some problems in the derived class to access it in a bind declaration.
	std::size_t is_transfer_complete(const boost::system::error_code& error,
		std::size_t bytes_transferred);

	void on_chunk_complete(const boost::system::error_code& error,
		std::size_t bytes_transferred);

protected:
	/** The buffer size is equal to the number of bytes to transfer.
	*/
	network_communicator(const buffer_type& buffer);

	/** The number of bytes to transfer is greater than the buffer size.
	*/
	network_communicator(const buffer_type& buffer, std::size_t bytes_to_transfer);

	buffer_type buffer_;
private:
	events_type events_;
	std::size_t bytes_to_transfer_;
	std::size_t bytes_transferred_;
};

template <class BufferSequence>
network_communicator<BufferSequence>::network_communicator(const buffer_type& buffer)
: buffer_(buffer)
, bytes_to_transfer_(boost::asio::buffer_size(buffer))
, bytes_transferred_(0)
{}

template <class BufferSequence>
network_communicator<BufferSequence>::network_communicator(const buffer_type& buffer, std::size_t bytes_to_transfer)
: buffer_(buffer)
, bytes_to_transfer_(bytes_to_transfer)
, bytes_transferred_(0)
{
	assert(boost::asio::buffer_size(buffer) <= bytes_to_transfer);
}

template <class BufferSequence>
std::size_t network_communicator<BufferSequence>::bytes_to_transfer() const
{
	return bytes_to_transfer_;
}

template <class BufferSequence>
std::size_t network_communicator<BufferSequence>::bytes_transferred() const
{
	return bytes_transferred_;
}

template <class BufferSequence>
bool network_communicator<BufferSequence>::is_done() const
{
	return bytes_to_transfer() == bytes_transferred();
}

template <class BufferSequence>
template <class Event, class F>
boost::signals2::connection network_communicator<BufferSequence>::on_event(F f)
{
	return events_.on_event<Event>(f);
}

template <class BufferSequence>
std::size_t network_communicator<BufferSequence>::is_transfer_complete(const boost::system::error_code& error,
	std::size_t bytes_transferred)
{
	bytes_transferred_ = bytes_transferred;
	if(error)
	{
		events_.signal_event<transfer_error>(error);
		return 0;
	}
	return bytes_to_transfer_ - bytes_transferred_;
}

template <class BufferSequence>
void network_communicator<BufferSequence>::on_chunk_complete(const boost::system::error_code& error,
	std::size_t bytes_transferred)
{
	bytes_transferred_ = bytes_transferred;
	if(error)
	{
		events_.signal_event<transfer_error>(error);
	}
	else
	{
		events_.signal_event<chunk_complete>(bytes_to_transfer_);
		if(bytes_transferred_ == bytes_to_transfer_)
		{
			events_.signal_event<transfer_complete>();
		}
	}
}

#endif // UMCD_NETWORK_COMMUNICATOR_HPP
