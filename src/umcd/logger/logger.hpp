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

#ifndef logger_HPP
#define logger_HPP

#include <ostream>
#include <iostream>
#include <sstream>
#include <map>

#include "config.hpp"
#include "umcd/logging_info.hpp"
#include "umcd/logger/severity_level.hpp"
#include "umcd/logger/detail/log_line_cache.hpp"

#include "umcd/boost/thread/workaround.hpp"
#include "umcd/boost/thread/lock_guard.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

namespace umcd{
namespace detail{
struct log_line;
struct log_stream;

} // namespace detail

class logger : boost::noncopyable
{
	static const char* severity_level_name[];

	typedef std::vector<detail::log_line> cache_type;
	typedef boost::shared_ptr<cache_type> cache_ptr;

	void default_logging_output();
	// Returns the old cache.
	cache_ptr make_new_cache();
	std::string make_header(severity_level sev) const;
	void set_log_output(const logging_info::severity_list& sev_list, const boost::shared_ptr<detail::log_stream>& stream);
	void set_standard_output(const logging_info::severity_list& sev_list, const std::ostream& stream);
	void set_files_output(const logging_info::file_list& files);

public:
	static std::map<std::string, severity_level> severity_str2enum;

	// Init map "textual representation of the severity level" to "severity level enum".
	static void init_severity_str2enum();
	logger();
	void add_line(const detail::log_line_cache& line);
	void run_once();
	void load(const logging_info& log_info);
	void set_severity(severity_level level);
	severity_level get_current_severity() const;
	void set_output(severity_level sev, const boost::shared_ptr<detail::log_stream>& stream);
	detail::log_line_cache get_logger(severity_level level);
private:
	severity_level current_sev_lvl_;
	boost::array<boost::shared_ptr<detail::log_stream>, nb_severity_level> logging_output_;
	boost::mutex cache_access_;
	boost::shared_ptr<cache_type> cache_;
};

class asio_logger
{
	asio_logger();

	/**
	@param local_timer is necessary if the sequence [run, stop, run] occurs too fast.
	*/
	void run_impl(boost::shared_ptr<boost::asio::deadline_timer> local_timer, const boost::system::error_code& error);

public:
	static asio_logger& get_asio_log();
	static logger& get();
	void run(boost::asio::io_service& io_service_, boost::posix_time::time_duration timing);
	void stop();

private:
	logger logger_;
	bool running_;
	boost::shared_ptr<boost::asio::deadline_timer> timer;
};

} // namespace umcd

#define CURRENT_FUNCTION_STRING "in " << BOOST_CURRENT_FUNCTION

#define UMCD_LOG(severity) (umcd::asio_logger::get().get_logger(severity))
#define UMCD_LOG_IP(severity, socket) ((umcd::asio_logger::get().get_logger(severity)) << socket.remote_endpoint())
#define UMCD_LOG_IP_FUNCTION_TRACER(socket) (UMCD_LOG_IP(trace, socket) << CURRENT_FUNCTION_STRING)
#define UMCD_LOG_FUNCTION_TRACER() (UMCD_LOG(trace) << CURRENT_FUNCTION_STRING)
#define RUN_ONCE_LOGGER() (umcd::asio_logger::get().run_once());

#endif // logger_HPP
