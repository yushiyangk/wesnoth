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

#include "umcd/special_packet.hpp"
#include "config.hpp"

namespace umcd{

static config make_special_packet(const std::string& message, const std::string& tag)
{
	config packet(tag);
	packet.child(tag)["text"] = "\"" + message + "\"";
	return packet;
}

config make_error_packet(const std::string& message)
{
	return make_special_packet(message, "error");
}

config make_warning_packet(const std::string& message)
{
	return make_special_packet(message, "warning");
}

} // namespace umcd