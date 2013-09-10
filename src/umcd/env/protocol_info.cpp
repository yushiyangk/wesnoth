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

#include "umcd/env/protocol_info.hpp"

namespace umcd{

std::size_t protocol_info::header_max_size_;
boost::signals2::signal<protocol_info::slot_type> protocol_info::signal_;

std::size_t protocol_info::header_max_size() const
{
	return header_max_size_;
}

void protocol_info::set_header_max_size(std::size_t header_size)
{
	header_max_size_ = header_size;
}

void protocol_info::on_header_max_size_change(boost::function<slot_type> slot_function)
{
	signal_.connect(slot_function);
}

} // namespace umcd
