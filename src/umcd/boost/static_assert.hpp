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

#ifndef UMCD_BOOST_STATIC_ASSERT_HPP
#define UMCD_BOOST_STATIC_ASSERT_HPP

#include <boost/static_assert.hpp>

// Some boost version doesn't propose this assert. 
// Forward it to BOOST_STATIC_ASSERT without printing the message.
#ifndef BOOST_STATIC_ASSERT_MSG
	#define BOOST_STATIC_ASSERT_MSG(x, m) BOOST_STATIC_ASSERT(x)
#endif

#endif // UMCD_BOOST_STATIC_ASSERT_HPP
