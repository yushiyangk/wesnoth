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

namespace umcd{

enum severity_level {
	trace,
	debug,
	info,
	warning,
	error,
	fatal,
	nb_severity_level
};

} // namespace umcd

#endif // UMCD_SEVERITY_LEVEL_HPP