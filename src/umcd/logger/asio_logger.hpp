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

#ifndef UMCD_ASIO_LOGGER_HPP
#define UMCD_ASIO_LOGGER_HPP

#include "umcd/logger/logger.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace umcd{

class asio_logger : 
    private boost::noncopyable
  , public boost::enable_shared_from_this<asio_logger>
{
	void run_impl(const boost::system::error_code& error);

public:
	static logger& get();

	asio_logger(boost::asio::io_service& io_service, boost::posix_time::time_duration time_between_log);
	void stop();

private:
	boost::shared_ptr<boost::asio::deadline_timer> timer;
};

} // namespace umcd

#define CURRENT_FUNCTION_STRING "in " << BOOST_CURRENT_FUNCTION

#define UMCD_LOG(lvl) (umcd::asio_logger::get().get_logger(umcd::severity::lvl))
#define UMCD_LOG_IP(lvl, socket_ptr) ((umcd::asio_logger::get().get_logger(umcd::severity::lvl)) << (socket_ptr)->remote_endpoint())
#define UMCD_LOG_IP_FUNCTION_TRACER(socket_ptr) (UMCD_LOG_IP(trace, socket_ptr) << CURRENT_FUNCTION_STRING)
#define UMCD_LOG_FUNCTION_TRACER() (UMCD_LOG(trace) << CURRENT_FUNCTION_STRING)
#define RUN_ONCE_LOGGER() (umcd::asio_logger::get().run_once());

#endif // UMCD_ASIO_LOGGER_HPP
