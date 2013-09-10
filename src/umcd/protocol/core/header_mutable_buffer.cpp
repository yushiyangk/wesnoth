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

#include "umcd/protocol/core/header_mutable_buffer.hpp"
#include "umcd/env/protocol_info.hpp"
#include "umcd/error.hpp"

#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

namespace umcd{
namespace core{

// header_mutable_buffer
header_mutable_buffer::header_mutable_buffer()
: base_type(buffer_type(NULL, 0))
, header_()
{
	buffer_ = boost::asio::buffer(reinterpret_cast<char*>(&header_.payload_size), sizeof(header_.payload_size));
	bytes_to_transfer_ = boost::asio::buffer_size(buffer_);
}

boost::shared_ptr<header_mutable_buffer::receiver_type> header_mutable_buffer::make_receiver(const socket_ptr& socket)
{
	boost::shared_ptr<receiver_type> receiver = boost::make_shared<receiver_type>(
		boost::cref(socket), 
		shared_from_this()
	);
	on_chunk_event_ = receiver->on_event<chunk_complete>(
		boost::bind(&header_mutable_buffer::make_metadata_buffer, this, _1));
	return receiver;
}

void header_mutable_buffer::make_metadata_buffer(transfer_events& ev)
{
	on_chunk_event_.disconnect();
	// Retreive the size and check if it's good.
	header_.payload_size = ntohl(header_.payload_size);
	protocol_info info;
	if(header_.payload_size > info.header_max_size())
	{
		ev.signal_event<transfer_error>(make_error_code(request_header_too_large));
	}
	else
	{
		// Dynamically updates buffer and bytes to transfer because we get this information now.
		header_.metadata.resize(header_.payload_size);
		buffer_ = boost::asio::buffer(&header_.metadata[0], header_.metadata.size());
		bytes_to_transfer_ += header_.metadata.size();
	}
}

header_data& header_mutable_buffer::data()
{
	return header_;
}

}} // namespace umcd::core
