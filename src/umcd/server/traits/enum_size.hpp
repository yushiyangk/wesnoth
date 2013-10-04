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

#ifndef UMCD_ENUM_SIZE_HPP
#define UMCD_ENUM_SIZE_HPP

/** Empty trait, it must be specialized and must expose a static const 
* integer member value representing the size of the enumeration EnumType.
*/
template <class EnumType>
struct enum_size
{};

#endif // UMCD_ENUM_SIZE_HPP
