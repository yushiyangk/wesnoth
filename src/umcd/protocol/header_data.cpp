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
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

namespace umcd{

boost::shared_ptr<header_data::sender_type> header_data::make_sender(socket_type& socket) const
{
	const_buffer_type buffers;
	buffers.push_back(boost::asio::buffer(reinterpret_cast<const char*>(&payload_size), sizeof(payload_size)));
	buffers.push_back(boost::asio::buffer(metadata));
	return boost::make_shared<sender_type>(boost::ref(socket), boost::cref(shared_from_this()), boost::cref(buffers));
}

boost::shared_ptr<header_data::receiver_type> header_data::make_receiver(socket_type& socket)
{
	boost::shared_ptr<receiver_type> receiver = boost::make_shared<receiver_type>(
		boost::ref(socket), 
		shared_from_this(),
		boost::asio::buffer(reinterpret_cast<char*>(&payload_size), sizeof(payload_size)));
	on_chunk_event_ = receiver->on_event(boost::bind(&header_data::make_metadata_buffer, shared_from_this(), _1, _2),
		event::chunk_complete);
	return receiver;
}

void header_data::make_metadata_buffer(mutable_buffer_type& buffer, std::size_t &bytes_to_transfer)
{
	on_chunk_event_.disconnect();
	// Retreive the size and check if it's good.
	payload_size = ntohl(payload_size);
	protocol_info info;
	if(payload_size > info.header_max_size())
	{
		//error = make_error_condition(request_header_too_large)
	}
	else
	{
		// Dynamically updates buffer and bytes to transfer because we get this information now.
		metadata.resize(payload_size);
		buffer = boost::asio::buffer(&metadata[0], metadata.size());
		bytes_to_transfer += metadata.size();
	}
}

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

boost::shared_ptr<header_data::sender_type> make_header_sender(boost::asio::ip::tcp::socket& socket, const config& metadata)
{
	boost::shared_ptr<header_data> header = boost::make_shared<header_data>();
	metadata >> *header;
	return header->make_sender(socket);
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
	header2data(const boost::shared_ptr<header_data>& header, Data& metadata)
	: header_(header)
	, metadata_(metadata)
	{}

	void load_data()
	{
		metadata_ << *header_;
	}

private:
	boost::shared_ptr<header_data> header_;
	Data& metadata_;
};
} // namespace detail

boost::shared_ptr<header_data::receiver_type> make_header_receiver(boost::asio::ip::tcp::socket& socket, config& metadata)
{
	boost::shared_ptr<header_data> header = boost::make_shared<header_data>();
	boost::shared_ptr<header_data::receiver_type> receiver = header->make_receiver(socket);

	typedef detail::header2data<config> header2config;
	boost::shared_ptr<header2config> h2c = boost::make_shared<header2config>(boost::cref(header), boost::ref(metadata));
	receiver->on_event(boost::bind(&header2config::load_data, h2c), event::transfer_complete);
	return receiver;
}

} // namespace umcd
