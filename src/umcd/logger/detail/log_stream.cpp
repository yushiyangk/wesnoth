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

#include "umcd/logger/detail/log_stream.hpp"

#include <boost/make_shared.hpp>
#include <fstream>
#include <ostream>
#include <ios>

namespace umcd{
namespace detail{

standard_log_stream::standard_log_stream(const std::ostream& log_stream)
: stream_(boost::make_shared<std::ostream>(log_stream.rdbuf()))
{}

boost::shared_ptr<std::ostream> standard_log_stream::stream()
{
	return stream_;
}

file_log_stream::file_log_stream(const std::string& filename)
: filename_(filename)
{}

boost::shared_ptr<std::ostream> file_log_stream::stream()
{
	return boost::make_shared<std::ofstream>(filename_.c_str(), std::ios_base::app);
}

}} // namespace umcd::detail
