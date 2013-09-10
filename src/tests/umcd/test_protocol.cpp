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
#include "umcd/client/client.hpp"
#include "umcd/protocol/make_header.hpp"
#include "config.hpp"
#include "game_config.hpp"
#include "serialization/parser.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <iostream>

using namespace umcd;

static void launch_test_suite(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket)
{
  boost::shared_ptr<send_receive_test> test_empty_license = boost::make_shared<send_receive_test>("empty license request", boost::cref(socket));
  test_empty_license->async_launch(
      game_config::path + "data/umcd/tests/request_license/request_license_empty_lang.cfg",
      game_config::path + "data/umcd/protocol_schema/request_license_reply.cfg");
}

static void on_failure(const boost::system::error_code& error)
{
  std::cout << "error: " << error.message() << std::endl;
}

static void on_success(const boost::shared_ptr<boost::asio::ip::tcp::socket>& socket)
{
  std::cout << "connected to " << socket->remote_endpoint() << std::endl;
  launch_test_suite(socket);
}

static void on_connect(const std::string& host_ip)
{
  std::cout << "Trying to connect at " << host_ip << std::endl;
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
  client c(io_service);
  c.on_event<try_connecting_with_ip>(on_connect);
  c.on_event<connection_success>(on_success);
  c.on_event<connection_failure>(on_failure);
  c.async_connect("localhost", "12522");
  io_service.run();

  std::cout << "quit\n";
  return 0;
}
