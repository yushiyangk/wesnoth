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

#ifndef UMCD_DATA_RECEIVER_HPP
#define UMCD_DATA_RECEIVER_HPP

#include "umcd/server/network_receiver.hpp"
#include <boost/shared_ptr.hpp>

template <class NetworkData, class MutableBufferSequence = typename NetworkData::mutable_buffer_type>
class data_receiver
: public network_receiver<MutableBufferSequence>
{
public:
	typedef network_receiver<MutableBufferSequence> base_type;
	typedef boost::asio::ip::tcp::socket socket_type;

	data_receiver(socket_type& socket, const boost::shared_ptr<NetworkData>& data, const MutableBufferSequence& buffer)
	: base_type(socket, buffer)
	, data_(data)
	{}
	
private:
	boost::shared_ptr<NetworkData> data_;
};

#endif // UMCD_DATA_RECEIVER_HPP
