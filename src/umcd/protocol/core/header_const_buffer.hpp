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

#ifndef UMCD_CORE_HEADER_CONST_BUFFER_HPP
#define UMCD_CORE_HEADER_CONST_BUFFER_HPP

#include "umcd/protocol/core/header_data.hpp"
#include "umcd/server/network_sender.hpp"
#include "umcd/server/buffer_provider.hpp"
#include <vector>

namespace umcd{
namespace core{

/** Provide a Boost.Asio const buffer compatible over the header_data.
* Use it to send data.
*/
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
	typedef boost::shared_ptr<socket_type> socket_ptr;

	header_const_buffer(const header_data& header);

	/** Create a sender that will use this buffer provider.
	* The data sent will be the header passed to the constructor.
	*/
	boost::shared_ptr<sender_type> make_sender(const socket_ptr& socket);

	const header_data& data() const;
private:
	header_data header_;
};

}} // namespace umcd::core

#endif // UMCD_CORE_HEADER_CONST_BUFFER_HPP
