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

#ifndef UMCD_DETAIL_LOG_LINE_CACHE_HPP
#define UMCD_DETAIL_LOG_LINE_CACHE_HPP

#include "umcd/logger/severity_level.hpp"

#include <boost/shared_ptr.hpp>
#include <sstream>

namespace umcd{
class logger;

namespace detail{
struct log_line;

class log_line_cache
{
private:
	friend struct log_line;

public:
	log_line_cache(logger& logger, severity::level severity_level);
	~log_line_cache();

	template <class Streamable>
	log_line_cache& operator<<(const Streamable& log)
	{
		if(enabled_)
		{
			 *line_ << log;
		}
		return *this;
	}

private:
	logger& logger_;
	bool enabled_;
	severity::level severity_;
	boost::shared_ptr<std::stringstream> line_;
};

}} // namespace umcd::detail

#endif // UMCD_DETAIL_LOG_LINE_CACHE_HPP