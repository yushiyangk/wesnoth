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

#include "umcd/env/environment_loader.hpp"
#include "umcd/env/logging_info.hpp"
#include "umcd/env/server_core.hpp"
#include "umcd/env/server_info.hpp"
#include "umcd/env/database_info.hpp"
#include "config.hpp"

#include <boost/algorithm/string.hpp>

void environment_loader::load(const config& cfg)
{
	load_server_core(cfg.child("server_core"));
	load_database_info(cfg.child("database"));
	load_server_info(cfg.child("server_info"));
	load_logging_info(cfg.child("logging"));
}

void environment_loader::load_server_core(const config& cfg)
{
	server_core sc;
	sc.set_port(cfg["port"]);
	sc.set_threads(cfg["threads"]);
}

void environment_loader::load_database_info(const config& cfg)
{
	database_info dc;
	dc.set_dsn(cfg["dsn"]);
	dc.set_user(cfg["user"]);
	dc.set_password(cfg["password"]);
}

void environment_loader::load_server_info(const config& cfg)
{
	server_info si;
	si.set_wesnoth_dir(cfg["wesnoth_dir"]);
}

logging_info::severity_list environment_loader::make_severity_list(const std::string& levels)
{
	std::vector<std::string> levels_to_stream;
	logging_info::severity_list sev_list;
	boost::algorithm::split(levels_to_stream, levels, boost::algorithm::is_any_of(" ,"));
	for(std::size_t i = 0; i < levels_to_stream.size(); ++i)
	{
		sev_list.push_back(umcd::severity::str2enum[levels_to_stream[i]]);
	}
	return sev_list;
}

logging_info::file_list environment_loader::make_file_list(const config& cfg)
{
	logging_info::file_list file_list;
	config::const_child_itors frange = cfg.child_range("file");
	for(;frange.first != frange.second; ++(frange.first))
	{
		const config& file_detail = *(frange.first);
		file_list.push_back(std::make_pair(file_detail["filename"].str(), make_severity_list(file_detail["level"])));
	}
	return file_list;
}

void environment_loader::load_logging_info(const config& cfg)
{
	umcd::severity::init_severity_str2enum();
	logging_info li;
	// Set the severity level.
	li.set_lower_limit(umcd::severity::str2enum[cfg["log_if_greater_or_equal"].str()]);
	if(cfg.has_child("cout"))
		li.set_to_cout(make_severity_list(cfg.child("cout")["level"]));
	if(cfg.has_child("cerr"))
		li.set_to_cerr(make_severity_list(cfg.child("cerr")["level"]));
	li.set_to_files(make_file_list(cfg));
}
