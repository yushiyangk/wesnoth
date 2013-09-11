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

#include "umcd/database.hpp"
#include "umcd/env/database_info.hpp"
#include "umcd/logger/asio_logger.hpp"
#include <boost/format.hpp>

namespace umcd{

database::otl_initializer database::otl_initializer_;
otl_connect database::db_;

database::otl_initializer::otl_initializer()
{
	otl_connect::otl_initialize();
}

database::database()
{
	if(!db_.connected)
	{
		database_info db_info;
		db_.rlogon(boost::str(boost::format("UID=%1%;PWD=%2%;DSN=%3%") % db_info.user() % db_info.password() % db_info.dsn()).c_str());
	}
}

otl_connect& database::db()
{
	return db_;
}

} // namespace umcd
