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

#include <boost/shared_ptr.hpp>
#include "config.hpp"
#include "umcd/actions/basic_umcd_action.hpp"
#include "umcd/env/server_info.hpp"

namespace umcd{

class request_umc_upload_action 
: public basic_umcd_action
{
public:
	typedef basic_umcd_action base;

	request_umc_upload_action(const server_info& info);
	const config& get_info(const config& metadata);

	virtual void execute(boost::shared_ptr<protocol> p);
	virtual boost::shared_ptr<base> clone() const;

private:
	server_info server_info_;
	boost::shared_ptr<protocol> protocol_;
};

} // namespace umcd
#endif // UMCD_REQUEST_UMC_UPLOAD_ACTION_HPP
