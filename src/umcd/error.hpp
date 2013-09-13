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

#ifndef UMCD_ERROR_HPP
#define UMCD_ERROR_HPP

#include <boost/system/error_code.hpp>
#include <boost/array.hpp>

namespace umcd{
enum error
{
	invalid_packet,
	invalid_packet_name,
	request_header_too_large,
	internal_error,
	bad_umc_id,
	field_too_long,
	num_error
};
} // namespace umcd

namespace boost{
namespace system{

template<>
struct is_error_condition_enum<umcd::error>
{
	static const bool value = true;
};

template<>
struct is_error_code_enum<umcd::error>
{
	static const bool value = true;
};
}} // namespace boost::system

namespace umcd{
class error_category : public boost::system::error_category
{
	static const boost::array<std::string, num_error> error_messages;
public:
	const char* name() const;
	std::string message(int ev) const;
};
} // namespace umcd

boost::system::error_condition make_error_condition(umcd::error e);
boost::system::error_code make_error_code(umcd::error e);
const boost::system::error_category& umcd_category();

#endif // UMCD_ERROR_HPP
