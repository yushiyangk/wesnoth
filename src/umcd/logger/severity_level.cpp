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

#include "umcd/logger/severity_level.hpp"

namespace umcd{
const char* severity::enum2str[] = {
	"trace",
	"debug",
	"info",
	"warning",
	"error",
	"fatal"
};

std::map<std::string, severity::level> severity::str2enum;

severity::str2enum_initializer::str2enum_initializer()
{
	for(int sev=0; sev < num_severity_level; ++sev)
		str2enum[enum2str[sev]] = static_cast<level>(sev);
}

severity::str2enum_initializer severity::str2enum_initializer_;

} // namespace umcd