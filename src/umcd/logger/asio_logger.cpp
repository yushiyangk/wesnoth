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

asio_logger::asio_logger()
: running_(false)
{}

/**
@param local_timer is necessary if the sequence [run, stop, run] occurs too fast.
*/
void asio_logger::run_impl(boost::shared_ptr<boost::asio::deadline_timer> local_timer, const boost::system::error_code& error)
{
	get().run_once();
	if(running_ && !error)
	{
		timer->async_wait(boost::bind(&asio_logger::run_impl, this,
			local_timer, boost::asio::placeholders::error));
	}
}

asio_logger& asio_logger::get_asio_log()
{
	static asio_logger lg;
	return lg;
}

logger& asio_logger::get()
{
	return get_asio_log().logger_;
}

void asio_logger::run(boost::asio::io_service& io_service_, boost::posix_time::time_duration timing)
{
	running_ = true;
	timer = boost::make_shared<boost::asio::deadline_timer>(boost::ref(io_service_), timing);
	timer->async_wait(boost::bind(&asio_logger::run_impl, this,
			timer, boost::asio::placeholders::error));
}

void asio_logger::stop()
{
	running_ = false;
	timer->cancel();
}

} // namespace umcd
