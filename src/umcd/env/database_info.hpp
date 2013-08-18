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

#ifndef UMCD_DATABASE_INFO_HPP
#define UMCD_DATABASE_INFO_HPP

#include <string>

class environment_loader;

class database_info
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

#endif // UMCD_DATABASE_INFO_HPP
