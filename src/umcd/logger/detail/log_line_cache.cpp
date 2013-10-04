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

#include "umcd/logger/detail/log_line_cache.hpp"
#include "umcd/logger/logger.hpp"

#include <boost/make_shared.hpp>

namespace umcd{
namespace detail{

log_line_cache::log_line_cache(logger& logger, severity::level severity_level)
: logger_(logger)
, enabled_(logger.get_current_severity() <= severity_level)
, severity_(severity_level)
, line_(boost::make_shared<std::stringstream>())
{}

log_line_cache::~log_line_cache()
{
	if(enabled_)
	{
		logger_.add_line(*this);
	}
}

}} // namespace umcd::detail
