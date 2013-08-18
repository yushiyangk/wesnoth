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

// NOTE: Please use the Boost.Log when the supported Boost version will be >= 1.54.

#ifndef UMCD_LOGGER_HPP
#define UMCD_LOGGER_HPP

#include <ostream>
#include <iostream>
#include <sstream>

#include "umcd/env/logging_info.hpp"
#include "umcd/logger/severity_level.hpp"
#include "umcd/logger/detail/log_line_cache.hpp"

#include "umcd/boost/thread/workaround.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

namespace umcd{
namespace detail{
struct log_line;
struct log_stream;

} // namespace detail

class logger : boost::noncopyable
{
	typedef std::vector<detail::log_line> cache_type;
	typedef boost::shared_ptr<cache_type> cache_ptr;

	void default_logging_output();
	// Returns the old cache.
	cache_ptr make_new_cache();
	std::string make_header(severity::level sev) const;
	void set_log_output(const logging_info::severity_list& sev_list, const boost::shared_ptr<detail::log_stream>& stream);
	void set_standard_output(const logging_info::severity_list& sev_list, const std::ostream& stream);
	void set_files_output(const logging_info::file_list& files);

public:
	logger();
	void add_line(const detail::log_line_cache& line);
	void run_once();
	void load(const logging_info& log_info);
	void set_severity(severity::level level);
	severity::level get_current_severity() const;
	void set_output(severity::level sev, const boost::shared_ptr<detail::log_stream>& stream);
	detail::log_line_cache get_logger(severity::level level);
private:
	severity::level current_sev_lvl_;
	boost::array<boost::shared_ptr<detail::log_stream>, severity::num_severity_level> logging_output_;
	boost::mutex cache_access_;
	boost::shared_ptr<cache_type> cache_;
};

} // namespace umcd

#endif // UMCD_LOGGER_HPP
