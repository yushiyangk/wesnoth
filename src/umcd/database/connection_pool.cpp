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

#include "umcd/database/connection_pool.hpp"
#include "umcd/database/connection_instance.hpp"

#include "umcd/boost/thread/lock_guard.hpp"
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <cassert>

namespace umcd{

connection_pool::connection_pool(std::size_t pool_size
		, boost::posix_time::time_duration wait_connection_timeout
		, const std::string& db_connect_info)
: pool_(pool_size)
, connection_availability_(pool_size)
, wait_connection_timeout_(wait_connection_timeout)
, db_connect_info_(db_connect_info)
{
	assert(pool_size > 0);
	for(std::size_t i = 0; i < pool_.size(); ++i)
	{
		connection_availability_.push_back(i);
		pool_[i] = boost::make_shared<otl_connect>();
	}
}

void connection_pool::dispatch_query(boost::asio::io_service &io_service
		, query_function_type query
		, timeout_function_type on_timeout)
{
	// Note: Be careful about this lock, it is not wrapped and, for example, an 
	// exception could made the mutex pool_access_ locking forever.
	// The code locked by this mutex is *exception free*.
	pool_access_.lock();
	// If no connection available.
	if(connection_availability_.empty())
	{
		pool_access_.unlock();
		// We add the task to the pending tasks queue with a timeout.
		lock_guard<boost::mutex> pending_tasks_lock(pending_tasks_access_);
		pending_tasks_.push_back(pending_task_type(
			  query 
			, boost::make_shared<boost::asio::deadline_timer>(boost::ref(io_service), wait_connection_timeout_))
		);
		pending_tasks_.back().timer->async_wait(
			boost::bind(&connection_pool::pending_task_timeout, this, on_timeout, _1));
	}
	else
	{
		// Pick up a connection index and pop it from the vector.
		std::size_t connection_index = connection_availability_.back();
		connection_availability_.pop_back();
		pool_access_.unlock();
		// Dispatch the query request with the connection.
		io_service.dispatch(boost::bind(query, 
			boost::make_shared<connection_instance>(boost::ref(*this), connection_index))
		);
	}
}

void connection_pool::pending_task_timeout(
			timeout_function_type on_timeout
		, const boost::system::error_code& error)
{
	if(error != boost::asio::error::operation_aborted)
	{
		on_timeout();
	}
}

otl_connect& connection_pool::get_instance(std::size_t instance_no)
{
	otl_connect& connection = *pool_[instance_no];
	if(!connection.connected)
		connection.rlogon(db_connect_info_.c_str());
	return connection;
}

void connection_pool::notify_release(std::size_t instance_no)
{
	pending_tasks_access_.lock();
	if(pending_tasks_.empty())
	{
		pending_tasks_access_.unlock();
		lock_guard<boost::mutex> pool_lock(pool_access_);
		connection_availability_.push_back(instance_no);
	}
	else
	{
		pending_task_type pending_task = pending_tasks_.front();
		pending_tasks_.pop_front();
		pending_tasks_access_.unlock();
		boost::system::error_code cancel_error;
		pending_task.timer->cancel(cancel_error);
		if(!cancel_error)
		{
			// Why a post and not a dispatch?
			// A dispatch means that the pending_task.task may be executed
			// in this thread and delayed the current operation for non-expected
			// work. Furthermore, pending_task.task can do the same and a infinite boucle
			// can appear if all the pending_task.task are executed in cascad in this thread.
			pending_task.timer->get_io_service().post(
				boost::bind(pending_task.query,
					boost::make_shared<connection_instance>(boost::ref(*this), instance_no))
			);
		}
	}
}

connection_pool::~connection_pool()
{
	for(std::size_t i = 0; i < pool_.size(); ++i)
		pool_[i]->logoff();
}

connection_pool::pending_task_type::pending_task_type(
		query_function_type query
	, const boost::shared_ptr<boost::asio::deadline_timer>& timer)
: query(query)
, timer(timer)
{}

} // namespace umcd
