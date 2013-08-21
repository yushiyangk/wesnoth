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

#include "umcd/server/data_sender.hpp"
#include <vector>

class config;

namespace umcd{

struct header_data : public boost::enable_shared_from_this<header_data>
{
	typedef std::vector<boost::asio::const_buffers_1> const_buffer_type;
	typedef boost::asio::mutable_buffers_1 mutable_buffer_type;

	typedef data_sender<header_data> sender_type;
	typedef boost::asio::ip::tcp::socket socket_type;

	boost::shared_ptr<sender_type> make_sender(socket_type& socket) const;
	// boost::shared_ptr<receiver_type> make_receiver(socket_type& socket);

	boost::uint32_t payload_size;
	std::string metadata;
};

header_data& operator>>(const config& metadata, header_data& header);
config& operator<<(config& metadata, const header_data& header);
boost::shared_ptr<header_data::sender_type> make_header_sender(boost::asio::ip::tcp::socket& socket, const config& metadata);

} // namespace umcd

#endif // UMCD_HEADER_DATA_HPP
