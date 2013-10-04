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

#ifndef SQL_SEMANTIC_ACTIONS_HPP
#define SQL_SEMANTIC_ACTIONS_HPP

#include "tools/code_generator/sql2cpp/sql/ast.hpp"

#include <boost/make_shared.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>

#define MAKE_PTR_LIMIT_ARGS 5

namespace sql{

class semantic_actions
{
public:
#define MAKE_PTR(z, count, unused) 													\
	template <class T, 																				\
						class U BOOST_PP_COMMA_IF(count) 								\
						BOOST_PP_ENUM_PARAMS(count, class Arg) >       	\
	void make_ptr(boost::shared_ptr<U>& res 									\
								BOOST_PP_COMMA_IF(count)										\
								BOOST_PP_ENUM_BINARY_PARAMS(count, Arg, const& arg) \
							 )const 																			\
	{																													\
		res = boost::make_shared<T>(BOOST_PP_ENUM_PARAMS(count, arg)); \
	}
	
BOOST_PP_REPEAT(MAKE_PTR_LIMIT_ARGS, MAKE_PTR, ~)

#undef MAKE_PTR

	void make_unsigned_numeric(boost::shared_ptr<type::numeric_type>& res) const;
	/**
	@param success is set to false to make the parser fails.
	*/
	void get_table_by_name(std::vector<ast::table>::iterator &res, 
		std::vector<ast::table>& tables, 
		const std::string& name,
		bool &success) const;

	/**
	@post We replace the constraint if it already exists in the table constraints, otherwise we add it.
	*/
	void alter_table_add_constraint(ast::table& table, 
		const boost::shared_ptr<sql::base_constraint>& constraint_to_add) const;
};

} // namespace sql
#endif // SQL_SEMANTIC_ACTIONS_HPP
