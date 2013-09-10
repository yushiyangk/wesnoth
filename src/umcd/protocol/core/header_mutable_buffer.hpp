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

#ifndef UMCD_CORE_HEADER_MUTABLE_BUFFER_HPP
#define UMCD_CORE_HEADER_MUTABLE_BUFFER_HPP

#include "umcd/protocol/core/header_data.hpp"
#include "umcd/server/network_receiver.hpp"
#include "umcd/server/buffer_provider.hpp"

namespace umcd{
namespace core{

/** Provide a Boost.Asio mutable buffer compatible over the header_data.
* Use it to receive data.
*/
class header_mutable_buffer
: public buffer_provider<boost::asio::mutable_buffers_1>
{
	typedef buffer_provider<boost::asio::mutable_buffers_1> base_type;
public:
	typedef boost::asio::mutable_buffers_1 buffer_type;
	typedef network_receiver<base_type> receiver_type;
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

	header_mutable_buffer();

	/** Create a receiver that will use this buffer provider.
	* The data received will be stored in the header local to this class.
	*/
	boost::shared_ptr<receiver_type> make_receiver(const socket_ptr& socket);

	/** Retreive the data you asked to receive. You should access it only when
	* the transfer_complete event is triggered. Register this event from the receiver
  * created with make_receiver.
	*/
	header_data& data();

private:
	/** This is an event handler that is called when the first chunk is read.
	* We can read the metadata before we get the payload and thus know how much
	* data there are to read.
	*/
	void make_metadata_buffer(transfer_events& ev);

	boost::signals2::connection on_chunk_event_;
	header_data header_;
};

}} // namespace umcd::core

#endif // UMCD_CORE_HEADER_MUTABLE_BUFFER_HPP
