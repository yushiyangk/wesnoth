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

#ifndef UMCD_ENVIRONMENT_HPP
#define UMCD_ENVIRONMENT_HPP

#include <boost/noncopyable.hpp>
#include <string>

#include "umcd/server/generic_factory.hpp"
#include "umcd/actions/basic_umcd_action.hpp"
#include "umcd/request_info.hpp"
#include "umcd/env/logging_info.hpp"

class config;

class environment : private boost::noncopyable
{
	typedef generic_factory<request_info> action_factory_type;
	typedef schema_validation::schema_validator validator_type;
	
	template <class Action>
	void register_request_info(const std::string& request_name);

public:
	environment(const server_info& info);

	boost::shared_ptr<request_info> get_request_info(const std::string& request_name) const;

private:
	server_info server_info_;
	action_factory_type action_factory_;
};

template <class Action>
void environment::register_request_info(const std::string& request_name)
{
	 action_factory_.register_product(
			request_name, 
			make_request_info<Action, validator_type>(server_info_, request_name)
	 );
}

class environment_loader;

class server_core
{
public:
	std::size_t threads() const;
	const std::string& port() const;

protected:
	friend class environment_loader;

	void set_threads(std::size_t server_threads);
	void set_port(const std::string& server_port);

private:
	static std::size_t threads_;
	static std::string port_;
};

class database_connexion
{
public:
	const std::string& dsn() const;
	const std::string& user() const;
	const std::string& password() const;

protected:
	friend class environment_loader;

	void set_dsn(const std::string& dsn);
	void set_user(const std::string& user);
	void set_password(const std::string& password);

private:
	static std::string dsn_;
	static std::string user_;
	static std::string password_;
};

class environment_loader
{
public:
	void load(const config& cfg);

private:
	void load_server_core(const config& cfg);
	void load_database_connexion(const config& cfg);
	void load_server_info(const config& cfg);
	void load_logging_info(const config& cfg);

	logging_info::severity_list make_severity_list(const std::string& levels);
	logging_info::file_list make_file_list(const config& cfg);
};

#endif // UMCD_ENVIRONMENT_HPP