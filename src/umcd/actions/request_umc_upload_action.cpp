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

#include "umcd/actions/request_umc_upload_action.hpp"
#include "umcd/env/server_info.hpp"
#include "umcd/otl/otl.hpp"
#include "filesystem.hpp"
#include "config.hpp"

namespace umcd{

const config& request_umc_upload_action::get_info(const config& metadata)
{
	return metadata.child("request_umc_upload").child("umc_configuration").child("info");
}

void request_umc_upload_action::execute(const socket_ptr&, const config&)
{
}

request_umc_upload_action::~request_umc_upload_action(){}

} // namespace umcd
