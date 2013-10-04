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

#include "umcd/protocol/core/header_data_stream.hpp"
#include "umcd/protocol/core/header_data.hpp"

#include "config.hpp"
#include "serialization/parser.hpp"

#include <sstream>

namespace umcd{
namespace core{

header_data& operator>>(const config& metadata, header_data& header)
{
	header.metadata = metadata.to_string();
	header.payload_size = header.metadata.size();
	return header;
}

config& operator<<(config& metadata, const header_data& header)
{
	std::stringstream header_stream(header.metadata);
	::read(metadata, header_stream);
	return metadata;
}

}} // namespace umcd::core
