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

environment::environment(const config& server_config)
: server_config_(server_config)
{
	register_request_info<request_license_action>("request_license");
	register_request_info<request_umc_upload_action>("request_umc_upload");
}

boost::shared_ptr<request_info> environment::get_request_info(const std::string& request_name) const
{
	return action_factory_.make_product(request_name);
}

std::size_t server_connexion::threads_ = 0;
std::string server_connexion::port_ = "10250";

std::size_t server_connexion::threads() const
{
	return threads_;
}

const std::string& server_connexion::port() const
{
	return port_;
}

void server_connexion::set_threads(std::size_t server_threads)
{
	threads_ = server_threads;
}

void server_connexion::set_port(const std::string& server_port)
{
	port_ = server_port;
}

void environment_loader::load(const config& cfg)
{
	load_server_connexion(cfg.child("server_core"));
}

void environment_loader::load_server_connexion(const config& cfg)
{
	server_connexion sc;
	sc.set_port(cfg["port"]);
	sc.set_threads(cfg["threads"]);
}
