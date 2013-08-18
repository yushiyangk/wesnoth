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

#ifndef UMCD_DETAIL_LOG_LINE_HPP
#define UMCD_DETAIL_LOG_LINE_HPP

#include "umcd/logger/severity_level.hpp"

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace umcd{
namespace detail{

struct log_line_cache;

struct log_line
{
	severity::level severity_level;
	std::string data;
	boost::posix_time::ptime time;

	log_line(const log_line_cache& cache_line);
};

}} // namespace umcd::detail

#endif // UMCD_DETAIL_LOG_LINE_HPP