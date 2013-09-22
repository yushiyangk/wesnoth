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

#ifndef UMCD_REQUEST_UMC_UPLOAD_ACTION_HPP
#define UMCD_REQUEST_UMC_UPLOAD_ACTION_HPP

#include "umcd/actions/basic_action.hpp"
#include "serialization/schema_validator.hpp"
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class config;
class otl_connect;
namespace pod{
	struct addon;
	struct addon_type;
	struct addon_version;
	struct language;
}
namespace umcd{

class connection_instance;

/** Analyse the request (pbl file) and if it seems correct, tell the client to send
* the compressed UMC file.
*/
class request_umc_upload_action 
: public basic_action_crtp<request_umc_upload_action>
, public boost::enable_shared_from_this<request_umc_upload_action>
{
public:
	typedef boost::asio::ip::tcp::socket socket_type;
	typedef boost::shared_ptr<socket_type> socket_ptr;

	virtual void execute(const socket_ptr& socket, const config& request);
	virtual ~request_umc_upload_action();

private:
	/** Helper to get the info section in the metadata.
	*/
	const config& get_info(const config& metadata);
	/** Helper to get the language section in the metadata.
	*/
	const config& get_lang(const config& metadata);

	/** Handle a database timeout. An error is asynchronously send on the socket.
	*/
	void on_db_timeout(const socket_ptr& socket);

	/** Update the UMC described in the request.
	* Asynchronously send an error if the request contains an invalid ID.
	* @note Must be completed, it only searches for an add-on.
	*/
	void update_umc(const boost::shared_ptr<connection_instance>& db_connection, const socket_ptr& socket, const config& request);
	
	/** Create the UMCD described in the request if it not exists yet.
	* Add the add-on and addon_version table in the database.
	* @note Not complete yet.
	*/
	void create_umc(const boost::shared_ptr<connection_instance>& db_connection, const socket_ptr& socket, const config& request);

	// Database queries.

	/**
	* @return Return the add-on type row described by the name addon_type_name.
	* @note This cannot failed (unless a database connection occurred). The addon_type_name
	* should have been checked in the WML schema and thus being valid.
	* @throw runtime_error if the addon_type_name is not correct.
	*/
	pod::addon_type retreive_addon_type_by_name(otl_connect& db, const std::string& addon_type_name);

	/**
	* @return Return the add-on language row described by the name language_name.
	* @note This cannot failed (unless a database connection occurred). The language_name
	* should have been checked in the WML schema and thus being valid.
	* @throw runtime_error if the language_name is not correct.
	*/
	pod::language retreive_language_by_name(otl_connect& db, const std::string& language_name);

	/** Retreive the add-on with id ID.
	* @return An optional add-on that is disengaged if the id was wrong, otherwise it's engaged with the add-on row.
	*/
	boost::optional<pod::addon> retreive_addon_by_id(otl_connect& db, boost::uint32_t id);

	/** Add the add-on row addon in the database.
	*/
	void add_addon(otl_connect& db, pod::addon& addon);
	
	/** Add the addon version row addon_version in the database.
	*/
	void add_addon_version(otl_connect& db, pod::addon_version& addon_version);
};

} // namespace umcd
#endif // UMCD_REQUEST_UMC_UPLOAD_ACTION_HPP
