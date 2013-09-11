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

#include "tests/umcd/send_receive_test.hpp"
#include "config.hpp"
#include "game_config.hpp"
#include "serialization/parser.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <iostream>

static void launch_test(boost::asio::io_service &io_service
  , const std::string& test_name
  , const std::string& data_to_send_filename
  , const std::string& expected_schema_filename)
{
  boost::shared_ptr<send_receive_test> test = boost::make_shared<send_receive_test>(
      test_name
    , data_to_send_filename
    , expected_schema_filename);
  test->async_launch(io_service);
}

static void launch_common_request_test_suite(boost::asio::io_service& io_service)
{
  launch_test(io_service
    , "bad request name"
    , game_config::path + "data/umcd/tests/common/bad_request_name.cfg"
    , game_config::path + "data/umcd/protocol_schema/error_reply.cfg");

  launch_test(io_service
    , "empty request"
    , game_config::path + "data/umcd/tests/common/empty_reply.cfg"
    , game_config::path + "data/umcd/protocol_schema/error_reply.cfg");
}

static void launch_license_request_test_suite(boost::asio::io_service& io_service)
{
  launch_test(io_service
    , "empty license request"
    , game_config::path + "data/umcd/tests/request_license/request_license_empty_lang.cfg"
    , game_config::path + "data/umcd/protocol_schema/request_license_reply.cfg");

  launch_test(io_service
    , "english license request"
    , game_config::path + "data/umcd/tests/request_license/request_license_english.cfg"
    , game_config::path + "data/umcd/protocol_schema/request_license_reply.cfg");
}

static void launch_upload_request_test_suite(boost::asio::io_service& io_service)
{
  launch_test(io_service
    , "upload new UMC"
    , game_config::path + "data/umcd/tests/request_umc_upload/request_umc_upload_basic.cfg"
    , "");
}

static void launch_test_suite(boost::asio::io_service& io_service)
{
  launch_common_request_test_suite(io_service);
  launch_license_request_test_suite(io_service);
  launch_upload_request_test_suite(io_service);
}

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    std::cerr << "usage: " << argv[0] << " wesnoth-dir" << std::endl;
    return 1;
  }
  game_config::path = argv[1];
  if(game_config::path.size() > 0 && *game_config::path.rbegin() != '/')
    game_config::path += '/';

  boost::asio::io_service io_service;
  launch_test_suite(io_service);
  io_service.run();

  std::cout << "quit\n";
  return 0;
}
