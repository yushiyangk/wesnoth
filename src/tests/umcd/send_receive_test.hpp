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

#ifndef UMCD_SEND_RECEIVE_TEST
#define UMCD_SEND_RECEIVE_TEST

#include "serialization/schema_validator.hpp"

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/lexical_cast.hpp>
#include <string>

class send_receive_test : 
  public boost::enable_shared_from_this<send_receive_test>
{
public:
  send_receive_test(const std::string& test_name
    , const std::string& data_to_send_filename
    , const std::string& expected_schema_filename);

  /**
  * @pre You should not call this function more than once in a multithread environment 
  * because the socket member is unsafe when shared.
  */
  void async_launch(boost::asio::io_service& io_service);

private:
  void on_connection_failure(const boost::system::error_code& error);
  void on_try_to_connect(const std::string& host_ip);
  void on_connection_success(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket);

  void begin_transfer();
  void async_receive();
  void on_receive_complete();
  void on_error(const std::string &from_function, const boost::system::error_code& error);
  void error_printer(const std::string &from_function, const std::string &error_message) const;
  void success_printer() const;
  std::string test_no_str() const;

  typedef schema_validation::schema_validator validator_type;

  std::string data_to_send_filename_;
  std::string expected_schema_filename_;
  std::string test_name_;
  std::size_t test_no_;
  config response_;
  boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;

  static std::size_t test_num_;
};

#endif // UMCD_SEND_RECEIVE_TEST
