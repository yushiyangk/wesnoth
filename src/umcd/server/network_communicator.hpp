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

/** @file Provides a generic representation of a event driven transfer
* (eventually splitted into chunks).
*/

#ifndef UMCD_NETWORK_COMMUNICATOR_HPP
#define UMCD_NETWORK_COMMUNICATOR_HPP

#include "umcd/server/transfer_events.hpp"

#include <boost/enable_shared_from_this.hpp>
#include <cassert>

#include "umcd/boost/asio/asio.hpp"

/** This class is a generic representation of a communication over a network.
* It provides some logic to split data transfer into chunks.
* You register event action with the on_event method and the events are only
* triggered by this class OR the buffer provider.
*
* + transfer_complete is triggered when all the chunks has been transferred according
* to the buffer provider.
* + transfer_error is triggered if an error has occurs during data transfer.
* + transfer_on_going is triggered from times to times when the number of bytes to transfer has
* changed.
* + chunk_complete is triggered when a chunk of data has been transferred. The details
* of what size have a chunk, or if the transfer is split into chunks depends on the buffer provider.
*
* You must inherit from this class. But you must not use this class for polymorphic purpose
* as the destructor is non virtual.
*
* @see network_receiver network_sender network_transfer buffer_provider
*/
template <class TransferOpCRTP, class BufferProvider>
class network_communicator
	: private boost::noncopyable
	, public boost::enable_shared_from_this<network_communicator<TransferOpCRTP, BufferProvider> >
{
public:
	typedef typename BufferProvider::buffer_type buffer_type;

	/**
	* @return The number of bytes that remains to transfer. This number can
	* possibly be dynamically updated by the buffer provider.
	*/
	std::size_t bytes_to_transfer() const;
	std::size_t bytes_transferred() const;
	bool is_done() const;
	buffer_type use_buffer() const;

	/** The slot function f will be triggered each time the event Event is reported.
	*/
	template <class Event, class F>
	boost::signals2::connection on_event(F f);

	/** Tell the asynchronous operation whether the transfer is complete or not.
	* @return The number of bytes the current operation needs to transfer yet.
	* @note Should be protected but there is some problems in the derived class 
	* to access it in a bind declaration.
	*/
	std::size_t is_transfer_complete(const boost::system::error_code& error,
		std::size_t bytes_transferred);

	/** This method is called when the transfer is complete.
	* @note Should be protected but there is some problems in the derived class 
	* to access it in a bind declaration.
	*/
	void on_chunk_complete(const boost::system::error_code& error,
		std::size_t bytes_transferred);

protected:
	/** The buffer size is equal to the number of bytes to transfer.
	*/
	network_communicator(const boost::shared_ptr<BufferProvider>& buffer_provider);

	network_communicator(){}

private:
	typedef network_communicator<TransferOpCRTP, BufferProvider> this_type;

	void update_bytes_transferred(std::size_t bytes_in_buffer);
	bool has_error() const;
	void on_error(const boost::system::error_code&);

	transfer_events events_;
	boost::shared_ptr<BufferProvider> buffer_provider_;
	std::size_t bytes_transferred_;
	std::size_t bytes_chunk_transferred_;
	bool has_error_;
};

template <class TransferOpCRTP, class BufferProvider>
network_communicator<TransferOpCRTP, BufferProvider>::network_communicator(const boost::shared_ptr<BufferProvider>& buffer_provider)
: buffer_provider_(buffer_provider)
, bytes_transferred_(0)
, bytes_chunk_transferred_(0)
, has_error_(false)
{
	/** We subscribe to the on_error event in case the buffer provider reports one.
	* It helps to not signal a complete transfer if an error has occur while the 
	* buffer provider changed the buffer.
	*/
	events_.on_event<transfer_error>(boost::bind(&this_type::on_error, this, _1));
}

template <class TransferOpCRTP, class BufferProvider>
void network_communicator<TransferOpCRTP, BufferProvider>::on_error(const boost::system::error_code&)
{
	has_error_ = true;
}

template <class TransferOpCRTP, class BufferProvider>
std::size_t network_communicator<TransferOpCRTP, BufferProvider>::bytes_to_transfer() const
{
	return buffer_provider_->bytes_to_transfer();
}

template <class TransferOpCRTP, class BufferProvider>
std::size_t network_communicator<TransferOpCRTP, BufferProvider>::bytes_transferred() const
{
	return bytes_transferred_;
}

template <class TransferOpCRTP, class BufferProvider>
bool network_communicator<TransferOpCRTP, BufferProvider>::is_done() const
{
	return bytes_to_transfer() == bytes_transferred();
}

template <class TransferOpCRTP, class BufferProvider>
bool network_communicator<TransferOpCRTP, BufferProvider>::has_error() const
{
	return has_error_;
}

template <class TransferOpCRTP, class BufferProvider>
typename network_communicator<TransferOpCRTP, BufferProvider>::buffer_type 
network_communicator<TransferOpCRTP, BufferProvider>::use_buffer() const
{
	return buffer_provider_->use_buffer();
}

template <class TransferOpCRTP, class BufferProvider>
template <class Event, class F>
boost::signals2::connection network_communicator<TransferOpCRTP, BufferProvider>::on_event(F f)
{
	return events_.on_event<Event>(f);
}

// We ignore the error, it will be handled by the on_chunk_complete operation.
template <class TransferOpCRTP, class BufferProvider>
std::size_t network_communicator<TransferOpCRTP, BufferProvider>::is_transfer_complete(const boost::system::error_code& error,
	std::size_t bytes_in_buffer)
{
	update_bytes_transferred(bytes_in_buffer);
	events_.signal_event<transfer_on_going>(bytes_transferred(), bytes_to_transfer());
	if(!error)
		return bytes_to_transfer() - bytes_transferred();
	else
		return 0;
}

template <class TransferOpCRTP, class BufferProvider>
void network_communicator<TransferOpCRTP, BufferProvider>::on_chunk_complete(const boost::system::error_code& error,
	std::size_t bytes_in_buffer)
{
	update_bytes_transferred(bytes_in_buffer);
	if(error)
	{
		events_.signal_event<transfer_error>(error);
	}
	else
	{
		events_.signal_event<transfer_on_going>(bytes_transferred(), bytes_to_transfer());
		events_.signal_event<chunk_complete>(events_);
		if(is_done() && !has_error())
		{
			events_.signal_event<transfer_complete>();
		}
		else
		{
			// Prepare for the next chunk.
			bytes_chunk_transferred_ = 0;
			static_cast<TransferOpCRTP*>(this)->async_transfer_impl();
		}
	}
}

template <class TransferOpCRTP, class BufferProvider>
void network_communicator<TransferOpCRTP, BufferProvider>::update_bytes_transferred(std::size_t bytes_in_buffer)
{
	bytes_transferred_ += (bytes_in_buffer - bytes_chunk_transferred_);
	bytes_chunk_transferred_ = bytes_in_buffer;
}

#endif // UMCD_NETWORK_COMMUNICATOR_HPP
