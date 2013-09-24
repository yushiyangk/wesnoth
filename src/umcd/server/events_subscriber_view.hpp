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

/** @file Represents a transfer of data over the network.
*/

#ifndef UMCD_EVENTS_SUBSCRIBER_VIEW_HPP
#define UMCD_EVENTS_SUBSCRIBER_VIEW_HPP

/** This class propose a view of an event class. It only proposes the 
* on_event method.
*/
template <class Events>
class events_subscriber_view
{
public:
	events_subscriber_view(Events& events)
	: events_(events)
	{}

	template <class Event, class F>
	boost::signals2::connection on_event(F f)
	{
		return events_.on_event<Event>(f);
	}
private:
	Events &events_;
};

#endif // UMCD_EVENTS_SUBSCRIBER_VIEW_HPP
