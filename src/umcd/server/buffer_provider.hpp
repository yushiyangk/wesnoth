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

#ifndef UMCD_BUFFER_PROVIDER_HPP
#define UMCD_BUFFER_PROVIDER_HPP

#include "umcd/server/transfer_events.hpp"
#include <boost/enable_shared_from_this.hpp>

/** Provide an access to buffer and the *known* size to transfer.
* Meaning that this size can rise.
* You must inherit from this class.
*/
template <class Buffer>
class buffer_provider : public boost::enable_shared_from_this<buffer_provider<Buffer> >
{
public:
	typedef Buffer buffer_type;

	std::size_t bytes_to_transfer() const
	{
		return bytes_to_transfer_;
	}

	buffer_type use_buffer() const
	{
		return buffer_;
	}

protected:
	buffer_provider(){}
	~buffer_provider(){}

	/** This constructor is mainly for buffer that have no default constructor.
	*/
	buffer_provider(buffer_type buffer)
	: buffer_(buffer)
	{}

	buffer_type buffer_;
	std::size_t bytes_to_transfer_;
};

#endif // UMCD_BUFFER_PROVIDER_HPP
