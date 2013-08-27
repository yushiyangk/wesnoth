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

#include "umcd/server/multi_threaded/server_mt.hpp"
#include "umcd/boost/thread/workaround.hpp"
#include <boost/thread/thread.hpp>
#include <boost/make_shared.hpp>

server_mt::server_mt(const umcd::server_core& server_config, const boost::function<void(const socket_ptr&)> &request_handler)
: base_type(server_config, request_handler)
{
	thread_pool_size_ = server_config.threads();
	if(thread_pool_size_ == 0)
	{
		thread_pool_size_ = boost::thread::hardware_concurrency();
	}
}

void server_mt::run()
{
	// Create a pool of threads to run all of the io_services.
	std::vector<boost::shared_ptr<boost::thread> > threads;
	for (std::size_t i = 0; i < thread_pool_size_-1; ++i)
	{
		boost::shared_ptr<boost::thread> thread = boost::make_shared<boost::thread>(
					boost::bind(&base_type::run, this));
		threads.push_back(thread);
	}

	// This thread is also used.
	base_type::run();

	// Wait for all threads in the pool to exit.
	for (std::size_t i = 0; i < threads.size(); ++i)
		threads[i]->join();
}

std::size_t server_mt::thread_pool_size() const
{
	return thread_pool_size_;
}
