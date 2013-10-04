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

#ifndef UMCD_CORE_HEADER_DATA_HPP
#define UMCD_CORE_HEADER_DATA_HPP

#include <boost/cstdint.hpp>
#include <string>

namespace umcd{
namespace core{

/** Simple binary data representing the couple (payload, metadata).
* The metadata are WML formatted. 
*/
struct header_data
{
	boost::uint32_t payload_size;
	std::string metadata;
};

}} // namespace umcd::core

#endif // UMCD_CORE_HEADER_DATA_HPP
