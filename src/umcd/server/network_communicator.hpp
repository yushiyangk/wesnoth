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

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/signals2/signal.hpp>
#include <cassert>

namespace event{
struct transfer_complete_tag{};
struct transfer_error_tag{};
struct transfer_on_going_tag{}; // Useful for refresh.
struct chunk_complete_tag{};

const transfer_complete_tag transfer_complete = {};
const transfer_error_tag transfer_error = {};
const transfer_on_going_tag transfer_on_going = {};
const chunk_complete_tag chunk_complete = {};
}

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

	void on_event(boost::function<void()> slot_function, event::transfer_complete_tag);
	void on_event(boost::function<void (const boost::system::error_code&)> slot_function, event::transfer_error_tag);
	void on_event(boost::function<void (std::size_t, std::size_t)> slot_function, event::transfer_on_going_tag);
	void on_event(boost::function<void (buffer_type&)> slot_function, event::chunk_complete_tag);

protected:
	std::size_t is_transfer_complete(const boost::system::error_code& error,
		std::size_t bytes_transferred);

	void chunk_complete(const boost::system::error_code& error,
		std::size_t bytes_transferred);

	/** The buffer size is equal to 
	*/
	network_communicator(const buffer_type& buffer);

	/** The number of bytes to transfer is greater than the buffer size.
	*/
	network_communicator(const buffer_type& buffer, std::size_t bytes_to_transfer);

private:
	buffer_type buffer_;
	std::size_t bytes_to_transfer_;
	std::size_t bytes_transferred_;
	std::size_t blocks_to_transfer_;
	boost::signals2::signal<void ()> sig_transfer_complete_;
	boost::signals2::signal<void (const boost::system::error_code&)> sig_transfer_error_;
	boost::signals2::signal<void (std::size_t, std::size_t)> sig_transfer_on_going_;
	boost::signals2::signal<void (buffer_type&)> sig_chunk_complete_;
};

template <class BufferSequence>
network_communicator<BufferSequence>::network_communicator(const buffer_type& buffer)
: buffer_(buffer)
, bytes_to_transfer_(boost::asio::buffer_size(buffer))
, bytes_transferred_(0)
, blocks_to_transfer_(1)
{}

template <class BufferSequence>
network_communicator<BufferSequence>::network_communicator(const buffer_type& buffer, std::size_t bytes_to_transfer)
: buffer_(buffer)
, bytes_to_transfer_(bytes_to_transfer)
, bytes_transferred_(0)
, blocks_to_transfer_(bytes_to_transfer / boost::asio::buffer_size(buffer)
									 + ((bytes_to_transfer % boost::asio::buffer_size(buffer)) != 0))
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
void network_communicator<BufferSequence>::on_event(boost::function<void()> slot_function, event::transfer_complete_tag)
{
	sig_transfer_complete_.connect(slot_function);
}

template <class BufferSequence>
void network_communicator<BufferSequence>::on_event(boost::function<void (const boost::system::error_code&)> slot_function, event::transfer_error_tag)
{
	sig_transfer_error_.connect(slot_function);
}

template <class BufferSequence>
void network_communicator<BufferSequence>::on_event(boost::function<void (std::size_t, std::size_t)> slot_function, event::transfer_on_going_tag)
{
	sig_transfer_on_going_.connect(slot_function);
}

template <class BufferSequence>
void network_communicator<BufferSequence>::on_event(boost::function<void (buffer_type&)> slot_function, event::chunk_complete_tag)
{
	sig_chunk_complete_.connect(slot_function);
}

template <class BufferSequence>
std::size_t network_communicator<BufferSequence>::is_transfer_complete(const boost::system::error_code& error,
	std::size_t bytes_transferred)
{
	bytes_transferred_ = bytes_transferred;
	if(error)
	{
		sig_transfer_error_(error);
		return 0;
	}
	return bytes_to_transfer_ - bytes_transferred_;
}

template <class BufferSequence>
void network_communicator<BufferSequence>::chunk_complete(const boost::system::error_code& error,
	std::size_t bytes_transferred)
{
	bytes_transferred_ = bytes_transferred;
	if(error)
	{
		sig_transfer_error_(error);
	}
	else
	{
		sig_chunk_complete_(buffer_);
		if(bytes_transferred_ == bytes_to_transfer_)
		{
			sig_transfer_complete_();
		}
	}
}

#endif // UMCD_NETWORK_COMMUNICATOR_HPP
