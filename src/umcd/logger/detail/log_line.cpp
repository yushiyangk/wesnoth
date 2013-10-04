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

#include "umcd/logger/detail/log_line.hpp"
#include "umcd/logger/detail/log_line_cache.hpp"

namespace umcd{
namespace detail{

log_line::log_line(const log_line_cache& cache_line)
: severity_level(cache_line.severity_)
, data(cache_line.line_->str())
, time(boost::posix_time::second_clock::universal_time())
{}

}} // namespace umcd::detail
