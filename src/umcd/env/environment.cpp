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

#include "umcd/env/environment.hpp"
#include "umcd/actions/request_license_action.hpp"
#include "umcd/actions/request_umc_upload_action.hpp"
#include "umcd/logger/logger.hpp"

#include "config.hpp"

#include <boost/algorithm/string.hpp>

environment::environment(const server_info& info)
: server_info_(info)
{
	register_request_info<request_license_action>("request_license");
	register_request_info<request_umc_upload_action>("request_umc_upload");
}

boost::shared_ptr<request_info> environment::get_request_info(const std::string& request_name) const
{
	return action_factory_.make_product(request_name);
}
