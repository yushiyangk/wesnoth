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

#ifndef UMCD_ENVIRONMENT_LOADER_HPP
#define UMCD_ENVIRONMENT_LOADER_HPP

#include "umcd/env/logging_info.hpp"
#include <string>

class config;

namespace umcd{

class environment_loader
{
public:
	void init_once();
	void load(const config& cfg);

private:
	void load_server_core(const config& cfg);
	void load_database_info(const config& cfg);
	void load_server_info(const config& cfg);
	void load_logging_info(const config& cfg);

	logging_info::severity_list make_severity_list(const std::string& levels);
	logging_info::file_list make_file_list(const config& cfg);
};

} // namespace umcd
#endif // UMCD_ENVIRONMENT_LOADER_HPP
