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

#include "umcd/database/database_query.hpp"
#include "umcd/database/database_server_type.hpp"

namespace umcd{

#ifdef MYSQL_DATABASE_SERVER
const boost::array<std::string, query::num_query> database_query::queries_ = {{
	"select LAST_INSERT_ID()" // last_insert_id
}};
#endif

} // namespace umcd
