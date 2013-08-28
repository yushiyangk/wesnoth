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

#ifndef UMCD_SEVERITY_LEVEL_HPP
#define UMCD_SEVERITY_LEVEL_HPP

#include <map>
#include <string>

namespace umcd{

struct severity
{
	enum level
	{
		trace,
		debug,
		info,
		warning,
		error,
		fatal,
		num_severity_level
	};

	static const char* enum2str[];
	static std::map<std::string, severity::level> str2enum;
private:
	struct str2enum_initializer
	{
		str2enum_initializer();
	};
	static str2enum_initializer str2enum_initializer_;
};
} // namespace umcd

#endif // UMCD_SEVERITY_LEVEL_HPP