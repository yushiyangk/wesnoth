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

#ifndef UMCD_DATABASE_CONNECTION_POOL_HPP
#define UMCD_DATABASE_CONNECTION_POOL_HPP

#include "umcd/otl/otl.hpp"

#include "umcd/boost/thread/workaround.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <deque>
#include <string>

namespace umcd{

class connection_instance;

class connection_pool : public boost::noncopyable
{
public:
	typedef boost::function<void()> timeout_function_type;
	typedef boost::function<void(const boost::shared_ptr<connection_instance>&)> query_function_type;

private:
	friend class connection_instance;

	otl_connect& get_instance(std::size_t instance_no);
	void notify_release(std::size_t instance_no);
	void pending_task_timeout(
		  timeout_function_type on_timeout
		, const boost::system::error_code& error);

public:
	/** Create a pool of size pool_size.
	* @param wait_connection_timeout is the timeout to wait for a connection.
	* @connect_str is the string describing the connection, look at the doc of OTL for the exact format.
	*/
	connection_pool(std::size_t pool_size
		, boost::posix_time::time_duration wait_connection_timeout
		, const std::string& connect_str);

	/** Asynchronous wait for an available connection if none are available.
	* If one is available the dispatch method is called on io_service.
	* @param query is the function to call with the connection when it will be available.
	* @param on_timeout is the function to call when a timeout will be triggered.
	* @note We call query or on_timeout but not both.
	*/
	void dispatch_query(boost::asio::io_service &io_service
		, query_function_type query
		, timeout_function_type on_timeout);

	/** Disconnect every connection to the database.
	*/
	~connection_pool();

private:
	/** This pool contains all the connection and is responsible to maintain them alive
	* (speaking about object lifetime and not about the connection itself).
	* The otl_connect is inside a shared_ptr because it has no copy constructor and thus
	* makes impossible the initialization of the vector.
	*/
	std::vector<boost::shared_ptr<otl_connect> > pool_;

	/** We store the index of the available connections.
	* The thread lock the pool_access_ mutex and pop the index in the back, if the 
	* connection_availability_ is empty, then we must wait for a connection.
	*/
	boost::mutex pool_access_;
	std::vector<std::size_t> connection_availability_;

	/** The pending tasks queue is guarded by a mutex. It represents all the tasks
	* waiting for a connection.
	*/
	struct pending_task_type
	{
		pending_task_type(query_function_type query
			, const boost::shared_ptr<boost::asio::deadline_timer>& timer);

		query_function_type query;
		// It must be allocated on the heap in the case of the cancel operation occurring after
		// the deadline_timer erased from the pending_tasks_.
		boost::shared_ptr<boost::asio::deadline_timer> timer;
	};

	boost::mutex pending_tasks_access_;
	std::deque<pending_task_type> pending_tasks_;

	/** The maximum time we wait for a connection.
	*/
	boost::posix_time::time_duration wait_connection_timeout_;
	/** String describing the database connection info as specified in the OTL doc.
	*/
	std::string db_connect_info_;
};

} // namespace umcd

#endif // UMCD_DATABASE_CONNECTION_POOL_HPP
