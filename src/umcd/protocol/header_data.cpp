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

#include "umcd/protocol/header_data.hpp"
#include "config.hpp"
#include "serialization/parser.hpp"
#include "umcd/env/protocol_info.hpp"
#include "umcd/error.hpp"
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

namespace umcd{

// header_const_buffer
header_const_buffer::header_const_buffer(const header_data& header)
: header_(header)
{
	buffer_.push_back(boost::asio::buffer(reinterpret_cast<const char*>(&header_.payload_size), sizeof(header_.payload_size)));
	buffer_.push_back(boost::asio::buffer(header_.metadata));
	bytes_to_transfer_ = boost::asio::buffer_size(buffer_);
}

boost::shared_ptr<header_const_buffer::sender_type> header_const_buffer::make_sender(socket_type& socket)
{
	return boost::make_shared<sender_type>(boost::ref(socket), boost::cref(this->shared_from_this()));
}

const header_data& header_const_buffer::data() const
{
	return header_;
}

// header_mutable_buffer
header_mutable_buffer::header_mutable_buffer()
: base_type(buffer_type(NULL, 0))
{
	buffer_ = boost::asio::buffer(reinterpret_cast<char*>(&header_.payload_size), sizeof(header_.payload_size));
	bytes_to_transfer_ = boost::asio::buffer_size(buffer_);
}

boost::shared_ptr<header_mutable_buffer::receiver_type> header_mutable_buffer::make_receiver(socket_type& socket)
{
	boost::shared_ptr<receiver_type> receiver = boost::make_shared<receiver_type>(
		boost::ref(socket), 
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

// Helpers to transform a config into a string and a string into a config.
header_data& operator>>(const config& metadata, header_data& header)
{
	header.metadata = metadata.to_string();
	header.payload_size = header.metadata.size();
	return header;
}

config& operator<<(config& metadata, const header_data& header)
{
	std::stringstream header_stream(header.metadata);
	::read(metadata, header_stream);
	return metadata;
}
/** The receiving is a bit more tricky because the conversion raw data to config is done at the end.
* We subscribe to the event transfer_complete and when the transfer will be complete the helper class header2data
* will automatically fill the config class. Because we are the first to subscribe, we are in the front, so no one
* will be notify of the transfer complete before us (and so it's safe for them to use the config data).
*/
namespace detail{
template <class Data>
class header2data
{
public:
	header2data(const boost::shared_ptr<header_mutable_buffer>& header_buf, Data& metadata)
	: header_buf_(header_buf)
	, metadata_(metadata)
	{}

	void load_data()
	{
		metadata_ << header_buf_->data();
	}

private:
	boost::shared_ptr<header_mutable_buffer> header_buf_;
	Data& metadata_;
};
} // namespace detail

// Factories
boost::shared_ptr<header_const_buffer::sender_type> make_header_sender(boost::asio::ip::tcp::socket& socket, const config& metadata)
{
	header_data data;
	metadata >> data;
	boost::shared_ptr<header_const_buffer> header = boost::make_shared<header_const_buffer>(data);
	return header->make_sender(socket);
}

boost::shared_ptr<header_mutable_buffer::receiver_type> make_header_receiver(boost::asio::ip::tcp::socket& socket, config& metadata)
{
	boost::shared_ptr<header_mutable_buffer> header = boost::make_shared<header_mutable_buffer>();
	boost::shared_ptr<header_mutable_buffer::receiver_type> receiver = header->make_receiver(socket);

	typedef detail::header2data<config> header2config;
	boost::shared_ptr<header2config> h2c = boost::make_shared<header2config>(boost::cref(header), boost::ref(metadata));
	receiver->on_event<transfer_complete>(boost::bind(&header2config::load_data, h2c));
	return receiver;
}

} // namespace umcd
