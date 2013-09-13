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

#ifndef UMCD_DATABASE_CONNECTION_INSTANCE_HPP
#define UMCD_DATABASE_CONNECTION_INSTANCE_HPP

#include "umcd/otl/otl.hpp"
#include <boost/noncopyable.hpp>

namespace umcd{

class connection_pool;

class connection_instance : public boost::noncopyable
{
private:
	friend class connection_pool;

public:
	connection_instance(connection_pool& pool, std::size_t instance_no);
	~connection_instance();
	otl_connect& get();
	
private:
	connection_pool& pool_;
	std::size_t instance_no_;
};

} // namespace umcd

#endif // UMCD_DATABASE_CONNECTION_INSTANCE_HPP
