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


#include "umcd/client/client.hpp"
// #include "game_config.hpp"
// #include "serialization/parser.hpp"

// #include <fstream>
#include <iostream>

using namespace umcd;

void on_failure(const boost::system::error_code& error);
void on_failure(const boost::system::error_code& error)
{
  std::cout << "error: " << error.message() << std::endl;
}

void on_success(const std::string& host_ip);
void on_success(const std::string& host_ip)
{
  std::cout << "Connected to " << host_ip << std::endl;
}

void on_connect(const std::string& host_ip);
void on_connect(const std::string& host_ip)
{
  std::cout << "Trying to connect at " << host_ip << std::endl;
}

int main()
{
  boost::asio::io_service io_service;
  client c(io_service);
  c.on_event<try_connecting_with_ip>(on_connect);
  c.on_event<connection_success>(on_success);
  c.on_event<connection_failure>(on_failure);
  c.async_connect("localhost", "12522");
  io_service.run();
  return 0;
}