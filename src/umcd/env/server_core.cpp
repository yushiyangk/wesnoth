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

#include "umcd/env/server_core.hpp"

std::size_t server_core::threads_;
std::string server_core::port_;

std::size_t server_core::threads() const
{
	return threads_;
}

const std::string& server_core::port() const
{
	return port_;
}

void server_core::set_threads(std::size_t server_threads)
{
	threads_ = server_threads;
}

void server_core::set_port(const std::string& server_port)
{
	port_ = server_port;
}
