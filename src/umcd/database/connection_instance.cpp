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

#include "umcd/database/connection_instance.hpp"
#include "umcd/database/connection_pool.hpp"

namespace umcd{

connection_instance::connection_instance(connection_pool& pool, std::size_t instance_no)
: pool_(pool)
, instance_no_(instance_no)
{}

connection_instance::~connection_instance()
{
	get().commit();
	pool_.notify_release(instance_no_);
}

otl_connect& connection_instance::get()
{
	return pool_.get_instance(instance_no_);
}

} // namespace umcd
