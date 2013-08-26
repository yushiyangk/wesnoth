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

#include "umcd/logger/asio_logger.hpp"

#include <boost/make_shared.hpp>

namespace umcd{

asio_logger::asio_logger(boost::asio::io_service& io_service, boost::posix_time::time_duration time_between_log)
: timer(boost::make_shared<boost::asio::deadline_timer>(boost::ref(io_service), time_between_log))
{
	timer->async_wait(boost::bind(&asio_logger::run_impl, this, boost::asio::placeholders::error));
}

void asio_logger::run_impl(const boost::system::error_code& error)
{
	get().run_once();
	if(!error)
	{
		timer->async_wait(boost::bind(&asio_logger::run_impl, this, boost::asio::placeholders::error));
	}
}

logger& asio_logger::get()
{
	static logger logger_;
	return logger_;
}

void asio_logger::stop()
{
	timer->cancel();
}

} // namespace umcd
