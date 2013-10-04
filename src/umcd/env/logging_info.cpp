/*
	Copyright (C) 2013 by Pierre Talbot <ptalbot@hyc.io>
	Part of the Battle for Wesnoth Project http://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#include "umcd/env/logging_info.hpp"

namespace umcd{

severity::level logging_info::lower_limit_;
logging_info::severity_list logging_info::cout_;
logging_info::severity_list logging_info::cerr_;
logging_info::file_list logging_info::files_;

severity::level logging_info::lower_limit() const
{
	return lower_limit_;
}

const logging_info::severity_list& logging_info::to_cout() const
{
	return cout_;
}

const logging_info::severity_list& logging_info::to_cerr() const
{
	return cerr_;
}

const logging_info::file_list& logging_info::to_files() const
{
	return files_;
}

void logging_info::set_lower_limit(severity::level lower_limit)
{
	lower_limit_ = lower_limit;
}

void logging_info::set_to_cout(const logging_info::severity_list& to_cout)
{
	cout_ = to_cout;
}

void logging_info::set_to_cerr(const logging_info::severity_list& to_cerr)
{
	cerr_ = to_cerr;
}

void logging_info::set_to_files(const logging_info::file_list& to_files)
{
	files_ = to_files;
}

} // namespace umcd
