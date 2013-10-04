//
// from file boost/asio/buffer.hpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
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

#ifndef UMCD_BOOST_ASIO_HPP
#define UMCD_BOOST_ASIO_HPP

#include <boost/asio.hpp>
#include <boost/version.hpp>

#if BOOST_VERSION < 104700

namespace boost{
namespace asio{
/// Get the total number of bytes in a buffer sequence.
/** 
 * The @c BufferSequence template parameter may meet either of the @c
 * ConstBufferSequence or @c MutableBufferSequence type requirements.
 */
template <typename BufferSequence>
inline std::size_t buffer_size(const BufferSequence& b)
{
  std::size_t total_buffer_size = 0;

  typename BufferSequence::const_iterator iter = b.begin();
  typename BufferSequence::const_iterator end = b.end();
  for (; iter != end; ++iter)
    total_buffer_size += detail::buffer_size_helper(*iter);

  return total_buffer_size;
}

}} // namespace boost::asio

#endif

#if BOOST_VERSION < 104800

namespace boost{
namespace asio{

// Workaround for the version where the async_connect free function didn't exist.
template <class Iterator, class ConnectCondition, class ConnectHandler>
void async_connect(boost::asio::ip::tcp::socket& socket,
				  Iterator endpoint_iterator
				, ConnectCondition /* not_used */
				, ConnectHandler connect_handler)
{
	socket.async_connect(*endpoint_iterator, boost::bind(connect_handler, _1, endpoint_iterator));
}

}}

#endif

#endif // UMCD_BOOST_ASIO_HPP