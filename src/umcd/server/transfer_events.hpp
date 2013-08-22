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

#ifndef UMCD_TRANSFER_EVENTS_HPP
#define UMCD_TRANSFER_EVENTS_HPP

#include "umcd/server/events.hpp"

struct transfer_complete{};
struct transfer_error{};

/** Use this event to track the transmission process.
*/
struct transfer_on_going{};
struct chunk_complete{}; // Useful to launch the next op.

template <>
struct event_slot<transfer_complete>
{
	/** A function declaration with no argument, just to notify the fact that the transfer is finished.
	*/
	typedef void type();
};

template <>
struct event_slot<transfer_error>
{
	/** A function declaration that takes an error_code and is called if the transmission fails.
	*/
	typedef void type(const boost::system::error_code&);
};

template <>
struct event_slot<transfer_on_going>
{
	/** A function declaration that takes the bytes transferred and the bytes to transfer (total).
	*/
	typedef void type(std::size_t, std::size_t);
};

struct transfer_events;

template <>
struct event_slot<chunk_complete>
{
	typedef void type(transfer_events&);
};

struct transfer_events
: events<boost::mpl::set<
				transfer_complete
			, transfer_error
			, transfer_on_going
			, chunk_complete> >
{};

#endif // UMCD_TRANSFER_EVENTS_HPP
