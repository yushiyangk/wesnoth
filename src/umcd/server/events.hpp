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

#ifndef UMCD_EVENTS_HPP
#define UMCD_EVENTS_HPP

#include <boost/mpl/set.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/sequence_tag.hpp>
#include <boost/signals2.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
#include <boost/static_assert.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#ifndef EVENT_LIMIT_ARG
	#define EVENT_LIMIT_ARG 5
#endif

template <class Event>
struct event_slot
{};

namespace detail{
namespace mpl = boost::mpl;

// For some reasons it's not available in Boost.MPL (no implementation defined...).
template <class Sequence>
struct pop_front
{
	typedef typename mpl::erase<
			Sequence
		, typename mpl::begin<Sequence>::type
	>::type type;
};

template <class EventSequence, bool empty = mpl::empty<EventSequence>::value>
class events_impl
{
	typedef EventSequence events_type;
	typedef typename mpl::front<events_type>::type event_type;
	typedef typename event_slot<event_type>::type event_slot_type;

public:
	template <class Event>
	typename boost::enable_if<
	    boost::is_same<Event, event_type>
		, boost::signals2::connection
	>::type on_event(boost::function<typename event_slot<Event>::type> slot_function)
	{
		return signal_.connect(slot_function);
	}

	template <class Event>
	typename boost::disable_if<
			boost::is_same<Event, event_type>
		, boost::signals2::connection
	>::type on_event(boost::function<typename event_slot<Event>::type> slot_function)
	{
		return events_tail_.template on_event<Event>(slot_function);
	}

#define MAKE_ARG_PARAM_TYPE_IMPL(count) typename boost::function_traits<typename event_slot<Event>::type>::arg##count##_type
#define MAKE_ARG_PARAM_TYPE(count) MAKE_ARG_PARAM_TYPE_IMPL(count)

#define MAKE_EVENT_ARG(z, count, unused) 		\
	BOOST_PP_COMMA_IF(count)									\
	MAKE_ARG_PARAM_TYPE(BOOST_PP_INC(count)) 	\
	arg##count

#define SIGNAL_EVENT(z, n, unused) 												\
	template <class Event>																	\
	typename boost::enable_if<															\
	    boost::is_same<Event, event_type>										\
		, void																								\
	>::type signal_event(BOOST_PP_REPEAT(n, MAKE_EVENT_ARG,~)) \
	{																												\
		signal_(BOOST_PP_ENUM_PARAMS(n, arg));								\
	}																												\
																													\
	template <class Event>																	\
	typename boost::disable_if<															\
			boost::is_same<Event, event_type>										\
		, void																								\
	>::type signal_event(BOOST_PP_REPEAT(n, MAKE_EVENT_ARG,~)) \
	{																												\
		events_tail_.template signal_event<Event>(BOOST_PP_ENUM_PARAMS(n, arg)); \
	}

BOOST_PP_REPEAT(EVENT_LIMIT_ARG, SIGNAL_EVENT, ~)

#undef SIGNAL_EVENT

private:
	boost::signals2::signal<event_slot_type> signal_;

	typedef typename pop_front<events_type>::type events_tail_type;
	events_impl<events_tail_type> events_tail_;
};

template <class EventSequence>
class events_impl<EventSequence, true>
{
public:
	template <class Event>
	boost::signals2::connection on_event(typename event_slot<Event>::type)
	{
		BOOST_STATIC_ASSERT_MSG(sizeof(Event) == 0, "** You are trying to access an empty sequence. Or to record a slot for an unknown event. **");
		return boost::signals2::connection();
	}
};

// We check that the sequence is a set.
template <class EventSequence, class sequence_tag>
class events_impl_tag
{
	BOOST_STATIC_ASSERT_MSG(sizeof(EventSequence) == 0, "** The sequence must be a boost::mpl::set. **");
};

template <class EventSequence>
class events_impl_tag<EventSequence, mpl::aux::set_tag>
: public events_impl<EventSequence>
{};
} // namespace detail


// Pre: EventSequence is a set and is non-empty.
template <class EventSequence>
class events
: public detail::events_impl_tag<
			EventSequence
		, typename boost::mpl::sequence_tag<EventSequence>::type>
{
};

#endif // UMCD_EVENTS_HPP
