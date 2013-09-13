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

#include "umcd/error.hpp"

namespace umcd{

const boost::array<std::string, num_error> error_category::error_messages = {{
	"The packet you sent is invalid. It could be a protocol bug and administrators have been contacted, the problem should be fixed soon.", // invalid_packet
	"The packet you sent has an invalid name. It could be a protocol bug and administrators have been contacted, the problem should be fixed soon.", // invalid_packet_name
	"The request you sent is too large. It can happens if you try to send a really large pbl file, you can claim help on IRC, and we'll find a solution.", // request_header_too_large
	"The server has encountered an unexpected error and your request cannot be processed.\n Congratz, you found a bug.", // internal_error
	"The ID inside your pbl file is unknown. No UMC found with this id.", // bad_umc_id
	"A field in your request is longer in size than allowed." // field_too_long
}};

const char* error_category::name() const
{
	return "umcd";
}

std::string error_category::message(int ev) const
{
	if(ev < 0 || ev >= static_cast<int>(error_messages.size()))
		return std::string("Unknown error");
	return error_messages[ev];
}

}// namespace umcd

// UMCD Error condition factory.
boost::system::error_condition make_error_condition(umcd::error e)
{
	return boost::system::error_condition(e, umcd_category());
}

// UMCD Error code factory.
boost::system::error_code make_error_code(umcd::error e)
{
	return boost::system::error_code(e, umcd_category());
}

const boost::system::error_category& umcd_category()
{
	static const umcd::error_category umcd_category_const;
	return umcd_category_const;
}
