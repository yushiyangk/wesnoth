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

#ifndef UMCD_BASIC_UMCD_ACTION_HPP
#define UMCD_BASIC_UMCD_ACTION_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class config;
namespace umcd{

class basic_action
{
public:
	typedef basic_action this_type;
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

	virtual void execute(const socket_ptr& socket, const config& request) = 0;
	virtual boost::shared_ptr<this_type> create() const = 0;
	virtual ~basic_action(){}
};

template <class ActionType>
class basic_action_crtp : public basic_action
{
public:
	virtual void execute(const socket_ptr& socket, const config& request) = 0;

	virtual boost::shared_ptr<basic_action> create() const
	{
		return boost::make_shared<ActionType>();
	}

	virtual ~basic_action_crtp(){}
};

} // namespace umcd

#endif // UMCD_BASIC_WML_ACTION_HPP
