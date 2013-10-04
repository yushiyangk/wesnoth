/*
	Copyright (C) 2013 by Pierre Talbot <ptalbot@hyc.io>
	Part of the Battle for Wesnoth Project http://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#include "umcd/protocol/core/header_const_buffer.hpp"

#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

namespace umcd{
namespace core{

header_const_buffer::header_const_buffer(const header_data& header)
: header_(header)
{
	header_.payload_size = htonl(header_.payload_size);
	buffer_.push_back(boost::asio::buffer(reinterpret_cast<const char*>(&header_.payload_size), sizeof(header_.payload_size)));
	buffer_.push_back(boost::asio::buffer(header_.metadata));
	bytes_to_transfer_ = boost::asio::buffer_size(buffer_);
}

boost::shared_ptr<header_const_buffer::sender_type> header_const_buffer::make_sender(const socket_ptr& socket)
{
	return boost::make_shared<sender_type>(boost::cref(socket), boost::cref(this->shared_from_this()));
}

}} // namespace umcd::core
