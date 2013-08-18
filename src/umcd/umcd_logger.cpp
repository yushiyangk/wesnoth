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

#include "umcd/umcd_logger.hpp"

log_line_cache::log_line_cache(umcd_logger& logger, severity_level severity)
: logger_(logger)
, enabled_(logger.get_current_severity() <= severity)
, severity_(severity)
, line_(boost::make_shared<std::stringstream>())
{}

log_line_cache::~log_line_cache()
{
	if(enabled_)
	{
		logger_.add_line(*this);
	}
}

log_line::log_line(const log_line_cache& cache_line)
: severity(cache_line.severity_)
, data(cache_line.line_->str())
, time(boost::posix_time::second_clock::universal_time())
{}

standard_log_stream::standard_log_stream(const std::ostream& log_stream)
: stream_(boost::make_shared<std::ostream>(log_stream.rdbuf()))
{}

boost::shared_ptr<std::ostream> standard_log_stream::stream()
{
	return stream_;
}

file_log_stream::file_log_stream(const std::string& filename)
: filename_(filename)
{}

boost::shared_ptr<std::ostream> file_log_stream::stream()
{
	return boost::make_shared<std::ofstream>(filename_.c_str(), std::ios_base::app);
}

const char* umcd_logger::severity_level_name[] = {
	"trace",
	"debug",
	"info",
	"warning",
	"error",
	"fatal"
};

std::map<std::string, severity_level> umcd_logger::severity_str2enum;

void umcd_logger::default_logging_output()
{
	int sev;
	for(sev=0; sev <= warning; ++sev)
	{
		set_output(static_cast<severity_level>(sev), boost::make_shared<standard_log_stream>(std::cout));
	}
	for(; sev < nb_severity_level; ++sev)
	{
		set_output(static_cast<severity_level>(sev), boost::make_shared<standard_log_stream>(std::cerr));
	}
}

// Returns the old cache.
umcd_logger::cache_ptr umcd_logger::make_new_cache()
{
	cache_ptr old_cache = cache_;
	lock_guard<boost::mutex> guard(cache_access_);
	cache_ = boost::make_shared<cache_type>();
	return old_cache;
}

std::string umcd_logger::make_header(severity_level sev) const
{
	return std::string("[") + severity_level_name[sev] + "] ";
}

void umcd_logger::set_log_output(const logging_info::severity_list& sev_list, const boost::shared_ptr<log_stream>& stream)
{
	for(std::size_t i = 0; i < sev_list.size(); ++i)
	{
		set_output(sev_list[i], stream);
	}
}

void umcd_logger::set_standard_output(const logging_info::severity_list& sev_list, const std::ostream& stream)
{
	set_log_output(sev_list, boost::make_shared<standard_log_stream>(stream));
}

void umcd_logger::set_files_output(const logging_info::file_list& files)
{
	for(std::size_t i=0; i < files.size(); ++i)
	{
		set_log_output(files[i].second, boost::make_shared<file_log_stream>(files[i].first));
	}
}

void umcd_logger::init_severity_str2enum()
{
	for(int sev=0; sev < nb_severity_level; ++sev)
		severity_str2enum[severity_level_name[sev]] = static_cast<severity_level>(sev);
}

umcd_logger::umcd_logger()
: current_sev_lvl_(trace)
, cache_(boost::make_shared<cache_type>())
{
	default_logging_output();
}

void umcd_logger::add_line(const log_line_cache& line)
{
	lock_guard<boost::mutex> guard(cache_access_);
	cache_->push_back(log_line(line));
}

void umcd_logger::run_once()
{
	cache_ptr old_cache = make_new_cache();
	boost::array<boost::shared_ptr<std::ostream>, nb_severity_level> log_streams;
	for(std::size_t i=0; i < nb_severity_level; ++i)
	{
		log_streams[i] = logging_output_[i]->stream();
	}

	for(std::size_t i=0; i < old_cache->size(); ++i)
	{
		const log_line& line = (*old_cache)[i];
		*log_streams[line.severity] << make_header(line.severity) 
			<< boost::posix_time::to_simple_string(line.time) << ": "
			<< line.data
			<< "\n";
	}
}

void umcd_logger::load(const logging_info& log_info)
{
	set_severity(log_info.lower_limit());

	set_standard_output(log_info.to_cout(), std::cout);
	set_standard_output(log_info.to_cerr(), std::cerr);
	set_files_output(log_info.to_files());
}

void umcd_logger::set_severity(severity_level level)
{
	current_sev_lvl_ = level;
}

severity_level umcd_logger::get_current_severity() const
{
	return current_sev_lvl_;
}

void umcd_logger::set_output(severity_level sev, const boost::shared_ptr<log_stream>& stream)
{
	logging_output_[sev] = stream;
}

log_line_cache umcd_logger::get_logger(severity_level level)
{
	return log_line_cache(*this, level);
}

asio_logger::asio_logger()
: running_(false)
{}

/**
@param local_timer is necessary if the sequence [run, stop, run] occurs too fast.
*/
void asio_logger::run_impl(boost::shared_ptr<boost::asio::deadline_timer> local_timer, const boost::system::error_code& error)
{
	get().run_once();
	if(running_ && !error)
	{
		timer->async_wait(boost::bind(&asio_logger::run_impl, this,
			local_timer, boost::asio::placeholders::error));
	}
}

asio_logger& asio_logger::get_asio_log()
{
	static asio_logger lg;
	return lg;
}

umcd_logger& asio_logger::get()
{
	return get_asio_log().logger_;
}

void asio_logger::run(boost::asio::io_service& io_service_, boost::posix_time::time_duration timing)
{
	running_ = true;
	timer = boost::make_shared<boost::asio::deadline_timer>(boost::ref(io_service_), timing);
	timer->async_wait(boost::bind(&asio_logger::run_impl, this,
			timer, boost::asio::placeholders::error));
}

void asio_logger::stop()
{
	running_ = false;
	timer->cancel();
}
