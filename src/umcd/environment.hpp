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

#ifndef UMCD_environment_HPP
#define UMCD_environment_HPP

#include <boost/noncopyable.hpp>
#include <string>

#include "umcd/request_info.hpp"
#include "umcd/server/generic_factory.hpp"
#include "umcd/actions/basic_umcd_action.hpp"

class config;

class environment : private boost::noncopyable
{
	typedef generic_factory<request_info> action_factory_type;
	typedef schema_validation::schema_validator validator_type;
	
	template <class Action>
	void register_request_info(const std::string& request_name);

public:
	environment(const config& server_config);

	boost::shared_ptr<request_info> get_request_info(const std::string& request_name) const;

private:
	config server_config_;
	action_factory_type action_factory_;
};

template <class Action>
void environment::register_request_info(const std::string& request_name)
{
	 action_factory_.register_product(
			request_name, 
			make_request_info<Action, validator_type>(server_config_, request_name)
	 );
}

class environment_loader;

class server_core
{
public:
	std::size_t threads() const;
	const std::string& port() const;

private:
	friend class environment_loader;

	void set_threads(std::size_t server_threads);
	void set_port(const std::string& server_port);

	static std::size_t threads_;
	static std::string port_;
};

class environment_loader
{
public:
	void load(const config& cfg);
private:
	void load_server_core(const config& cfg);
};

class database_connexion{};
class server_info{};
class logging_info{};

#endif // UMCD_environment_HPP