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

#ifndef UMCD_MAKE_HEADER_HPP
#define UMCD_MAKE_HEADER_HPP

#include "umcd/protocol/core/header_const_buffer.hpp"
#include "umcd/protocol/core/header_mutable_buffer.hpp"
#include "umcd/protocol/core/header_data_stream.hpp"
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>

namespace umcd{

/** Make a sender with a socket and the data to send. Subscribe to the transfer event to know
* when the transfer is completed or in case of any other change (consult the transfer_event class).
*
* @tparam DataType Must be streamable to a header_data type, by overloading the operator >>(const DataType&, header_data&).
*/
template <class DataType>
boost::shared_ptr<core::header_const_buffer::sender_type> make_header_sender(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket, const DataType& metadata);

/** Make a receiver with a socket and a storage in which we can receive data. 
* Subscribe to the transfer event to know when the transfer is completed or in case 
* of any other change (consult the transfer_event class).
*
* @tparam DataType Must be streamable from a header_data type, by overloading the operator <<(DataType&, const header_data&).
* @param metadata Must stay alive until you receive an event that notice the transfer ending (such as the completion or failure).
*/
template <class DataType>
boost::shared_ptr<core::header_mutable_buffer::receiver_type> make_header_receiver(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket, DataType& metadata);

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
	header2data(const boost::shared_ptr<core::header_mutable_buffer>& header_buf, Data& metadata)
	: header_buf_(header_buf)
	, metadata_(metadata)
	{}

	void load_data()
	{
		metadata_ << header_buf_->data();
	}

private:
	boost::shared_ptr<core::header_mutable_buffer> header_buf_;
	Data& metadata_;
};
} // namespace detail

// The sender factory.
template <class DataType>
boost::shared_ptr<core::header_const_buffer::sender_type> make_header_sender(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket, const DataType& metadata)
{
	core::header_data data;
	metadata >> data;
	boost::shared_ptr<core::header_const_buffer> header = boost::make_shared<core::header_const_buffer>(data);
	return header->make_sender(socket);
}

// The receiver factory.
template <class DataType>
boost::shared_ptr<core::header_mutable_buffer::receiver_type> make_header_receiver(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket, DataType& metadata)
{
	boost::shared_ptr<core::header_mutable_buffer> header = boost::make_shared<core::header_mutable_buffer>();
	boost::shared_ptr<core::header_mutable_buffer::receiver_type> receiver = header->make_receiver(socket);

	typedef detail::header2data<DataType> header2data;
	boost::shared_ptr<header2data> h2c = boost::make_shared<header2data>(boost::cref(header), boost::ref(metadata));
	receiver->on_event<transfer_complete>(boost::bind(&header2data::load_data, h2c));
	return receiver;
}

} // namespace umcd

#endif // UMCD_MAKE_HEADER_HPP
