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
#include <boost/make_shared.hpp>

namespace umcd{

boost::shared_ptr<header_data::sender_type> header_data::make_sender(socket_type& socket) const
{
	const_buffer_type buffers;
	buffers.push_back(boost::asio::buffer(reinterpret_cast<const char*>(&payload_size), sizeof(payload_size)));
	buffers.push_back(boost::asio::buffer(metadata));
	return boost::make_shared<sender_type>(boost::ref(socket), boost::cref(shared_from_this()), boost::cref(buffers));
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

} // namespace umcd
