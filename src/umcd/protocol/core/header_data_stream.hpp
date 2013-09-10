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

#ifndef UMCD_CORE_HEADER_DATA_STREAM_HPP
#define UMCD_CORE_HEADER_DATA_STREAM_HPP

class config;
class header_data;

namespace umcd{
namespace core{

/** Transform the WML metadata into binary data (header_data).
*/
header_data& operator>>(const config& metadata, header_data& header);

/** Transform the binary data (header_data) into readable WML data.
*/
config& operator<<(config& metadata, const header_data& header);

}} // namespace umcd::core

#endif // UMCD_CORE_HEADER_DATA_STREAM_HPP
