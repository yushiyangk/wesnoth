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

/** @file Provide a type erasure for the actions and a "CRTP action" used for common
* behavior such as the create method.
*/

#ifndef UMCD_BASIC_ACTION_HPP
#define UMCD_BASIC_ACTION_HPP

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class config;
namespace umcd{

/** Type erasure and define the interface of an action. Hence, an action is default
* constructible (via the create method) and executable (via the execute method).
*/
class basic_action
{
public:
	typedef basic_action this_type;
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

	/** Execute the logic behind an action. The action analyse the request and use the 
	* socket to send the response.
	*/
	virtual void execute(const socket_ptr& socket, const config& request) = 0;

	/** Create an action of the current sub-type.
	*/
	virtual boost::shared_ptr<this_type> create() const = 0;
	virtual ~basic_action(){}
};

/** CRTP on action for common behavior such as the creation.
*/
template <class ActionType>
class basic_action_crtp : public basic_action
{
public:
	/**
	* @see basic_action
	*/
	virtual void execute(const socket_ptr& socket, const config& request) = 0;

	virtual boost::shared_ptr<basic_action> create() const
	{
		return boost::make_shared<ActionType>();
	}

	virtual ~basic_action_crtp(){}
};

} // namespace umcd

#endif // UMCD_BASIC_ACTION_HPP
