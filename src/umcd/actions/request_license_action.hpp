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

/* Design rational:
The license is not shipped with the Wesnoth client because this server can be re-use with different licenses on other server than Wesnoth ones.
*/

#ifndef UMCD_REQUEST_LICENSE_ACTION_HPP
#define UMCD_REQUEST_LICENSE_ACTION_HPP

#include "umcd/actions/basic_action.hpp"
#include "serialization/schema_validator.hpp"

class config;

namespace umcd{

class request_license_action : 
  public basic_action_crtp<request_license_action>
{
	typedef schema_validation::schema_validator validator_type;

public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

	virtual void execute(const socket_ptr& socket, const config& request);
	virtual ~request_license_action();
};

} // namespace umcd
#endif // UMCD_REQUEST_LICENSE_ACTION_HPP
