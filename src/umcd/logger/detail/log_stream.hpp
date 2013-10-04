/*
	Copyright (C) 2013 by Pierre Talbot <ptalbot@hyc.io>
	Part of the Battle for Wesnoth Project http://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#ifndef UMCD_DETAIL_LOG_STREAM_HPP
#define UMCD_DETAIL_LOG_STREAM_HPP

#include <boost/shared_ptr.hpp>
#include <ostream>

namespace umcd{
namespace detail{

class log_stream
{
public:
	virtual ~log_stream() {}
	virtual boost::shared_ptr<std::ostream> stream() = 0;
};

class standard_log_stream : public log_stream
{
public:
	standard_log_stream(const std::ostream& log_stream);
	virtual boost::shared_ptr<std::ostream> stream();

private:
	boost::shared_ptr<std::ostream> stream_;
};

class file_log_stream : public log_stream
{
public:
	file_log_stream(const std::string& filename);
	virtual boost::shared_ptr<std::ostream> stream();

private:
	std::string filename_;
};

}} // namespace umcd::detail

#endif // UMCD_DETAIL_LOG_STREAM_HPP