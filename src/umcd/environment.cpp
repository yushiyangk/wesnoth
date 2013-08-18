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

#include "umcd/environment.hpp"
#include "umcd/actions/request_license_action.hpp"
#include "umcd/actions/request_umc_upload_action.hpp"
#include "umcd/logger/logger.hpp"

#include "config.hpp"

#include <boost/algorithm/string.hpp>

environment::environment(const server_info& info)
: server_info_(info)
{
	register_request_info<request_license_action>("request_license");
	register_request_info<request_umc_upload_action>("request_umc_upload");
}

boost::shared_ptr<request_info> environment::get_request_info(const std::string& request_name) const
{
	return action_factory_.make_product(request_name);
}

std::size_t server_core::threads_;
std::string server_core::port_;

std::size_t server_core::threads() const
{
	return threads_;
}

const std::string& server_core::port() const
{
	return port_;
}

void server_core::set_threads(std::size_t server_threads)
{
	threads_ = server_threads;
}

void server_core::set_port(const std::string& server_port)
{
	port_ = server_port;
}

std::string database_connexion::dsn_;
std::string database_connexion::user_;
std::string database_connexion::password_;

const std::string& database_connexion::dsn() const
{
	return dsn_;
}

const std::string& database_connexion::user() const
{
	return user_;
}

const std::string& database_connexion::password() const
{
	return password_;
}

void database_connexion::set_dsn(const std::string& dsn)
{
	dsn_ = dsn;
}

void database_connexion::set_user(const std::string& user)
{
	user_ = user;
}

void database_connexion::set_password(const std::string& password)
{
	password_ = password;
}

void environment_loader::load(const config& cfg)
{
	load_server_core(cfg.child("server_core"));
	load_database_connexion(cfg.child("database"));
	load_server_info(cfg.child("server_info"));
	load_logging_info(cfg.child("logging"));
}

void environment_loader::load_server_core(const config& cfg)
{
	server_core sc;
	sc.set_port(cfg["port"]);
	sc.set_threads(cfg["threads"]);
}

void environment_loader::load_database_connexion(const config& cfg)
{
	database_connexion dc;
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
		sev_list.push_back(logger::severity_str2enum[levels_to_stream[i]]);
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
	logger::init_severity_str2enum();
	logging_info li;
	// Set the severity level.
	li.set_lower_limit(logger::severity_str2enum[cfg["log_if_greater_or_equal"].str()]);
	if(cfg.has_child("cout"))
		li.set_to_cout(make_severity_list(cfg.child("cout")["level"]));
	if(cfg.has_child("cerr"))
		li.set_to_cerr(make_severity_list(cfg.child("cerr")["level"]));
	li.set_to_files(make_file_list(cfg));
}
