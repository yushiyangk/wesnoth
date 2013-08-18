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

#ifndef UMCD_LOGGING_INFO_HPP
#define UMCD_LOGGING_INFO_HPP

#include "umcd/logger/severity_level.hpp"

#include <string>
#include <vector>
#include <utility>

class environment_loader;

class logging_info
{
public:
	typedef std::vector<severity_level> severity_list;
	typedef std::vector<std::pair<std::string, severity_list> > file_list;

	severity_level lower_limit() const;
	const severity_list& to_cout() const;
	const severity_list& to_cerr() const;
	const file_list& to_files() const;

protected:
	friend class environment_loader;

	void set_lower_limit(severity_level lvl);
	void set_to_cout(const severity_list& to_cout);
	void set_to_cerr(const severity_list& to_cerr);
	void set_to_files(const file_list& to_files);

private:
	static severity_level lower_limit_;
	static severity_list cout_;
	static severity_list cerr_;
	static file_list files_;
};

#endif // UMCD_LOGGING_INFO_HPP