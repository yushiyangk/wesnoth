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

#include "umcd/env/database_info.hpp"

std::string database_info::dsn_;
std::string database_info::user_;
std::string database_info::password_;

const std::string& database_info::dsn() const
{
	return dsn_;
}

const std::string& database_info::user() const
{
	return user_;
}

const std::string& database_info::password() const
{
	return password_;
}

void database_info::set_dsn(const std::string& dsn)
{
	dsn_ = dsn;
}

void database_info::set_user(const std::string& user)
{
	user_ = user;
}

void database_info::set_password(const std::string& password)
{
	password_ = password;
}
