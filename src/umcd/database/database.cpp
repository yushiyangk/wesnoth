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

#include "umcd/database/database.hpp"
#include "umcd/env/database_info.hpp"
#include "umcd/logger/asio_logger.hpp"
#include <boost/format.hpp>
#include <cassert>
#include <iostream>

namespace umcd{

database::otl_initializer database::otl_initializer_;
boost::shared_ptr<connection_pool> database::connection_pool_;

database::otl_initializer::otl_initializer()
{
	otl_connect::otl_initialize(1);
}

void database::dispatch_query(boost::asio::io_service &io_service
	, connection_pool::query_function_type query
	, connection_pool::timeout_function_type on_timeout)
{
	assert(static_cast<bool>(connection_pool_));
	connection_pool_->dispatch_query(io_service, query, on_timeout);
}

void database::init_db(const database_info& db_info)
{
	connection_pool_ = boost::make_shared<connection_pool>(
			db_info.num_connection()
		, boost::posix_time::seconds(db_info.timeout_seconds())
		, boost::str(boost::format("UID=%1%;PWD=%2%;DSN=%3%") % db_info.user() % db_info.password() % db_info.dsn()).c_str()
	);
}

void dispatch_query(boost::asio::io_service &io_service
	, connection_pool::query_function_type query
	, connection_pool::timeout_function_type on_timeout)
{
	database db;
	db.dispatch_query(io_service, query, on_timeout);
}

} // namespace umcd
