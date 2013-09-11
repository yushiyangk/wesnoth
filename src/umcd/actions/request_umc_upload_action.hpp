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

class config;
class otl_connect;
namespace pod{
	struct addon;
	struct addon_type;
	struct language;
}
namespace umcd{

/** Analyse the request (pbl file) and if it seems correct, tell the client to send
* the compressed UMC file.
*/
class request_umc_upload_action 
: public basic_action_crtp<request_umc_upload_action>
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

	// Database queries.
	pod::addon_type retreive_addon_type_by_name(otl_connect& db, const std::string& addon_type_name);
	pod::language retreive_language_by_name(otl_connect& db, const std::string& language_name);
	boost::optional<pod::addon> retreive_addon_by_id(otl_connect& db, boost::uint32_t id);
};

} // namespace umcd
#endif // UMCD_REQUEST_UMC_UPLOAD_ACTION_HPP
