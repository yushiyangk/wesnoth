/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#include "umcd/protocol/action_dispatcher.hpp"

#include "umcd/actions/request_license_action.hpp"
#include "umcd/actions/request_umc_upload_action.hpp"
#include "umcd/protocol/header_data.hpp"
#include "umcd/protocol/close_on_error.hpp"
#include "umcd/protocol/error_sender.hpp"
#include "umcd/error.hpp"
#include "umcd/logger/asio_logger.hpp"
#include <boost/current_function.hpp>

namespace umcd{

action_dispatcher::action_factory_type action_dispatcher::action_factory;
action_dispatcher::action_factory_initializer action_dispatcher::action_init;

action_dispatcher::action_factory_initializer::action_factory_initializer()
{
	action_factory.register_product("request_license", boost::make_shared<request_license_action>());
	action_factory.register_product("request_umc_upload", boost::make_shared<request_umc_upload_action>());
}

void action_dispatcher::async_receive_request()
{
	boost::shared_ptr<header_mutable_buffer::receiver_type> receiver = make_header_receiver(socket_, header_metadata_);
	receiver->on_event<transfer_error>(boost::bind(&close_on_error, socket_, _1));
	receiver->on_event<transfer_complete>(boost::bind(&action_dispatcher::dispatch, shared_from_this()));
	receiver->async_receive();
}

void action_dispatcher::dispatch()
{
	UMCD_LOG_IP_FUNCTION_TRACER(socket_);
	try
	{
		// Retrieve request name.
		config::all_children_itors range = header_metadata_.all_children_range();
		if(range.first == range.second)
		{
			async_send_error(socket_, make_error_condition(invalid_packet_name));
		}
		else
		{
			const std::string& request_name = range.first->key;
			UMCD_LOG_IP(info, socket_) << " -- request name: " << request_name;
			UMCD_LOG_IP(trace, socket_) << " -- request header:\n" << header_metadata_;

			action_ptr action = action_factory.make_product(request_name);
			action->execute(header_metadata_);
		}
	}
	catch(const std::exception& e)
	{
		UMCD_LOG_IP(error, socket_) << " -- invalid request at " << BOOST_CURRENT_FUNCTION << " (" << e.what() << ")";
		async_send_error(socket_, make_error_condition(invalid_packet));
	}
}

} // namespace umcd
