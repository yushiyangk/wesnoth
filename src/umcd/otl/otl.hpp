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

#ifndef UMCD_OTL_HPP
#define UMCD_OTL_HPP

#define OTL_ODBC

// Use ODBC_UNIX if we are on Unix.
// You must install unixODBC: sudo apt-get install unixodbc-dev
#if defined(unix) || defined(__unix) || defined(__unix__)
	#define OTL_ODBC_UNIX
#endif

// Enable the use of the STL
#define OTL_STL

#include <boost/format.hpp>
// Derive otl_exception from std::exception
#define OTL_EXCEPTION_DERIVED_FROM std::exception
#define OTL_EXCEPTION_HAS_MEMBERS											\
	mutable std::string tmp_buf;												\
	virtual const char* what() const throw()						\
	{																										\
		tmp_buf = boost::str(boost::format(								\
			"\n\tDatabase error message: %1%\n"							\
			"\tDatabase SQL statement: %2%\n"								\
			"\tDatabase SQL state message: %3%\n"						\
			"\tDatabase variable: %4%")											\
		% this->msg % stm_text % this->sqlstate % var_info); \
		return tmp_buf.c_str();														\
	}

#define OTL_STREAM_THROWS_NOT_CONNECTED_TO_DATABASE_EXCEPTION

#ifdef HAVE_CXX11
	#define OTL_CPP_11_ON
#endif

// Catch beyond-the-end-of-file read error.
#define OTL_PARANOID_EOF

#include "umcd/otl/otlv4.h"

#include <boost/array.hpp>
// Specializing stream operator >> and << for the boost array.
template <std::size_t N>
otl_stream& operator>>(otl_stream& s, boost::array<char, N>& a)
{
	return s >> a.elems;
}

template <std::size_t N>
otl_stream& operator<<(otl_stream& s, boost::array<char, N>& a)
{
	return s << a.elems;
}

#include <boost/date_time/posix_time/posix_time.hpp>
// Add specialization for the boost::posix_time::ptime to otl_datetime struct.
template <class OTLStream>
OTLStream& operator<<(OTLStream& stream, const boost::posix_time::ptime& time)
{
	otl_datetime tm;

  tm.year = time.date().year();
  tm.month = time.date().month();
  tm.day = time.date().day();
  tm.hour = time.time_of_day().hours();
  tm.minute = time.time_of_day().minutes();
  tm.second = time.time_of_day().seconds();

  stream << tm;
  return stream;
}

#endif // UMCD_OTL_HPP
