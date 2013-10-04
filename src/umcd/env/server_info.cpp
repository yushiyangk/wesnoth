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

#include "umcd/env/server_info.hpp"
#include "game_config.hpp"

namespace umcd{
std::string server_info::wesnoth_dir_;

const std::string& server_info::wesnoth_dir() const
{
	return wesnoth_dir_;
}

void server_info::set_wesnoth_dir(const std::string& wesnoth_dir)
{
	wesnoth_dir_ = wesnoth_dir;
	game_config::path = wesnoth_dir_;
}

} // namespace umcd
