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

#ifndef UMCD_SERVER_INFO_HPP
#define UMCD_SERVER_INFO_HPP

#include <string>

namespace umcd{
class environment_loader;

class server_info
{
public:
	const std::string& wesnoth_dir() const;

protected:
	friend class environment_loader;

	void set_wesnoth_dir(const std::string& wesnoth_dir);

private:
	static std::string wesnoth_dir_;
};

} // namespace umcd
#endif // UMCD_SERVER_INFO_HPP