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


#ifdef HAVE_CXX11
	#define OTL_CPP_11_ON
#endif

// Extends the list of supported numeric data type.
#include <boost/cstdint.hpp>
#include <boost/lexical_cast.hpp>
#include <cassert>

// Extends for the boost::uint16_t.
#define OTL_NUMERIC_TYPE_1 boost::uint16_t
#define OTL_NUMERIC_TYPE_1_ID "uint16_t"
#define OTL_NUMERIC_TYPE_1_STR_SIZE 7
#define OTL_STR_TO_NUMERIC_TYPE_1(str,n)			\
{																							\
	n = boost::lexical_cast<boost::uint16_t> 		\
		(str, OTL_NUMERIC_TYPE_1_STR_SIZE);				\
}

#define OTL_NUMERIC_TYPE_1_TO_STR(n, str)			\
{																													\
	std::string tmp = boost::lexical_cast<std::string>(n);	\
	assert(tmp.size() < OTL_NUMERIC_TYPE_1_STR_SIZE);				\
	std::copy(tmp.begin(), tmp.end(), str);									\
}

// Catch beyond-the-end-of-file read error.
#define OTL_PARANOID_EOF

#include "umcd/otl/otlv4.h"

// Specializing stream operator >> and << for the boost array.
template <size_t N>
otl_stream& operator>>(otl_stream& s, boost::array<char, N>& a)
{
	return s >> a.elems;
}

template <size_t N>
otl_stream& operator<<(otl_stream& s, boost::array<char, N>& a)
{
	return s << a.elems;
}

#endif // UMCD_OTL_HPP
