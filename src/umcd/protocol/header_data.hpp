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

#ifndef UMCD_HEADER_DATA_HPP
#define UMCD_HEADER_DATA_HPP

#include "umcd/server/network_receiver.hpp"
#include "umcd/server/network_sender.hpp"
#include "umcd/server/buffer_provider.hpp"
#include <vector>

class config;

namespace umcd{

struct header_data
{
	boost::uint32_t payload_size;
	std::string metadata;
};

class header_const_buffer 
: public buffer_provider<std::vector<boost::asio::const_buffers_1> >
{
public:
	typedef std::vector<boost::asio::const_buffers_1> buffer_type;
private:
	typedef buffer_provider<buffer_type> base_type;
public:
	typedef network_sender<base_type> sender_type;
	typedef boost::asio::ip::tcp::socket socket_type;

	header_const_buffer(const header_data& header);
	boost::shared_ptr<sender_type> make_sender(socket_type& socket);

	const header_data& data() const;
private:
	header_data header_;
};

class header_mutable_buffer
: public buffer_provider<boost::asio::mutable_buffers_1>
{
	typedef buffer_provider<boost::asio::mutable_buffers_1> base_type;
public:
	typedef boost::asio::mutable_buffers_1 buffer_type;
	typedef network_receiver<base_type> receiver_type;
	typedef boost::asio::ip::tcp::socket socket_type;

	header_mutable_buffer();
	boost::shared_ptr<receiver_type> make_receiver(socket_type& socket);

	header_data& data();

private:
	void make_metadata_buffer(transfer_events& ev);

	boost::signals2::connection on_chunk_event_;
	header_data header_;
};

header_data& operator>>(const config& metadata, header_data& header);
config& operator<<(config& metadata, const header_data& header);
boost::shared_ptr<header_const_buffer::sender_type> make_header_sender(boost::asio::ip::tcp::socket& socket, const config& metadata);

/** You can subscribe to the event transfer_complete to know when metadata will be filled.
*/
boost::shared_ptr<header_mutable_buffer::receiver_type> make_header_receiver(boost::asio::ip::tcp::socket& socket, config& metadata);

} // namespace umcd

#endif // UMCD_HEADER_DATA_HPP
