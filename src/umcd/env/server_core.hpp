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

#ifndef UMCD_SERVER_CORE_HPP
#define UMCD_SERVER_CORE_HPP

#include <string>

namespace umcd{
class environment_loader;

class server_core
{
public:
	std::size_t threads() const;
	const std::string& port() const;

protected:
	friend class environment_loader;

	void set_threads(std::size_t server_threads);
	void set_port(const std::string& server_port);

private:
	static std::size_t threads_;
	static std::string port_;
};

} // namespace umcd
#endif // UMCD_SERVER_CORE_HPP
