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

#include "config.hpp"

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