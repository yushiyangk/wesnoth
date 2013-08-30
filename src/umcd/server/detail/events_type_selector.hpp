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

#ifndef UMCD_EVENTS_TYPE_SELECTOR_HPP
#define UMCD_EVENTS_TYPE_SELECTOR_HPP

#include "umcd/server/detail/events_enum_impl.hpp"
#include "umcd/server/detail/events_set_impl.hpp"
#include "umcd/server/traits/enum_size.hpp"

#include <boost/mpl/sequence_tag.hpp>

namespace detail{

/** Class select the underlining type of an event: enum or tags.
*/
template <class EventSequence, bool is_enum = boost::is_enum<EventSequence>::value >
class events_type_selector
: public events_set_impl_tag<
			EventSequence
		, typename boost::mpl::sequence_tag<EventSequence>::type>
{
};

template <class EventEnumType>
class events_type_selector<EventEnumType, true>
: public events_enum_impl<
			EventEnumType
		, enum_size<EventEnumType>::value >
{
};

} // namespace detail

#endif // UMCD_EVENTS_TYPE_SELECTOR_HPP
