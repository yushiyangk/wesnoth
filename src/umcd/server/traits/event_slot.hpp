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

#ifndef UMCD_EVENT_SLOT_HPP
#define UMCD_EVENT_SLOT_HPP

/** Empty trait, it must be specialized to link an event with a particular
* function slot signature. Must expose a function type named "type".
*/
template <class Event>
struct event_slot
{};

/** Empty trait, it must be specialized to link an event with a particular
* function slot signature. This event_slot_c traits is for enumeration-based events.
* Must expose a function type named "type".
*/
template <class EnumType, std::size_t Event>
struct event_slot_c
{};

#endif // UMCD_EVENT_SLOT_HPP
