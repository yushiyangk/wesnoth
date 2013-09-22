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

#ifndef UMCD_DATABASE_QUERY_HPP
#define UMCD_DATABASE_QUERY_HPP

#include <boost/array.hpp>
#include <string>

namespace umcd{
namespace query{
enum db_query_type
{
	last_inserted_id,
	num_query
};
}

/** Record each query that are specific to a database.
* These queries are accessed with the db_query_type enumeration.
*/
class database_query
{
public:
	template <std::size_t query_type>
	static const std::string& get()
	{
		return queries_[query_type];
	}

private:
	static const boost::array<std::string, query::num_query> queries_;
};

} // namespace umcd

#endif // UMCD_DATABASE_QUERY_HPP
