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

/** Simple binary data representing the couple (payload, metadata).
* The metadata are WML formatted. 
*/
struct header_data
{
	boost::uint32_t payload_size;
	std::string metadata;
};

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

/** Provide a Boost.Asio mutable buffer compatible over the header_data.
* Use it to receive data.
*/
class header_mutable_buffer
: public buffer_provider<boost::asio::mutable_buffers_1>
{
	typedef buffer_provider<boost::asio::mutable_buffers_1> base_type;
public:
	typedef boost::asio::mutable_buffers_1 buffer_type;
	typedef network_receiver<base_type> receiver_type;
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

	header_mutable_buffer();

	/** Create a receiver that will use this buffer provider.
	* The data received will be stored in the header local to this class.
	*/
	boost::shared_ptr<receiver_type> make_receiver(const socket_ptr& socket);

	/** Retreive the data you asked to receive. You should access it only when
	* the transfer_complete event is triggered. Register this event from the receiver
  * created with make_receiver.
	*/
	header_data& data();

private:
	/** This is an event handler that is called when the first chunk is read.
	* We can read the metadata before we get the payload and thus know how much
	* data there are to read.
	*/
	void make_metadata_buffer(transfer_events& ev);

	boost::signals2::connection on_chunk_event_;
	header_data header_;
};

/** Transform the WML metadata into binary data (header_data).
*/
header_data& operator>>(const config& metadata, header_data& header);

/** Transform the binary data (header_data) into readable WML data.
*/
config& operator<<(config& metadata, const header_data& header);

/** Make a sender with a socket and the data to send. Subscribe to the transfer event to know
* when the transfer is completed or in case of any other change (consult the transfer_event class).
*/
boost::shared_ptr<header_const_buffer::sender_type> make_header_sender(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket, const config& metadata);

/** Make a receiver with a socket and a storage in which we can receive data. 
* Subscribe to the transfer event to know when the transfer is completed or in case 
* of any other change (consult the transfer_event class).
*
* @param metadata Must stay alive until you receive an event that notice the transfer ending (such as the completion or failure).
*/
boost::shared_ptr<header_mutable_buffer::receiver_type> make_header_receiver(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket, config& metadata);

} // namespace umcd

#endif // UMCD_HEADER_DATA_HPP
