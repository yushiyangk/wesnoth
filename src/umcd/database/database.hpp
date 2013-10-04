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

#ifndef UMCD_DATABASE_HPP
#define UMCD_DATABASE_HPP

#include "umcd/otl/otl.hpp"
#include "umcd/database/connection_pool.hpp"

namespace umcd{

class database_info;

class database
{
public:
	/** Asynchronous wait for an available connection if none are available.
	* If one is available the dispatch method is called on io_service.
	* @param query is the function to call with the connection when it will be available.
	* @param on_timeout is the function to call when a timeout will be triggered.
	* @note We call query or on_timeout but not both.
	*/
	void dispatch_query(boost::asio::io_service &io_service
		, connection_pool::query_function_type query
		, connection_pool::timeout_function_type on_timeout);

	/** Initialized the static member in the database.
	*/
	static void init_db(const database_info& db_info);

private:
	struct otl_initializer
	{
		otl_initializer();
	};
	static otl_initializer otl_initializer_;
	static boost::shared_ptr<connection_pool> connection_pool_;
};

/** Asynchronous wait for an available connection if none are available.
* If one is available the dispatch method is called on io_service.
* @param query is the function to call with the connection when it will be available.
* @param on_timeout is the function to call when a timeout will be triggered.
* @note We call query or on_timeout but not both.
*/
void dispatch_query(boost::asio::io_service &io_service
	, connection_pool::query_function_type query
	, connection_pool::timeout_function_type on_timeout);

} // namespace umcd

#endif // UMCD_DATABASE_HPP
