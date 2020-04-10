/* lib@home, framework to develop distributed calculations.
 * Copyright (C) 2020  Dirk "YouDirk" Lehmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "libathome-common/Logger.hpp"


void libathome_common::Logger::
_init()
{
  this->strftime_fmt = "[%H:%M:%S]";

  this->info("*** Log session initialized; log-level=%s; timezone=%s",
             Logger::to_string(this->loglevel),
             RealtimeClock::to_string(this->timezone));
}

libathome_common::Logger::
Logger(Logger::loglevel_t loglevel, RealtimeClock::timezone_t timezone)
  :File(stdout, "<stdout>"), loglevel(loglevel), timezone(timezone),
   file_fmt("%Y-%m-%d.log"), file_count(365)
{
  this->_init();
}

libathome_common::Logger::
Logger(Logger::loglevel_t loglevel, RealtimeClock::timezone_t timezone,
  const std::string& path, const std::string& file_fmt,
  unsigned file_count)
  :File(path, "tmp.log", false), loglevel(loglevel), timezone(timezone),
   file_fmt(file_fmt), file_count(file_count)
{
  this->_init();
}

libathome_common::Logger::
~Logger()
{
  this->info("*** Log session closed, bye *zzz* ...");
}

libathome_common::Logger* libathome_common::Log = NULL;

/* ***************************************************************  */

const char* libathome_common::Logger::
to_string(Logger::loglevel_t loglevel)
{
  switch (loglevel) {
  case all_e: return "all";
  case debug_e: return "debug";
  case info_e: return "info";
  case warning_e: return "warning";
  case error_e: return "ERROR";
  case fatal_e: return "FATAL";
  case none_e: return "none";
  }

  return "<not implemented!>";
}

void libathome_common::Logger::
set_loglevel(Logger::loglevel_t loglevel)
{
  this->loglevel = loglevel;

  this->info("*** Log session set; log-level=%s",
    Logger::to_string(this->loglevel));
}

void libathome_common::Logger::
set_timezone(RealtimeClock::timezone_t timezone)
{
  this->debug("<-- Log session; timezone=%s",
    RealtimeClock::to_string(this->timezone));

  this->timezone = timezone;

  this->info("*** Log session set; timezone=%s",
    RealtimeClock::to_string(this->timezone));
}

libathome_common::Logger::loglevel_t libathome_common::Logger::
get_loglevel() const
{
  return this->loglevel;
}

libathome_common::RealtimeClock::timezone_t libathome_common::Logger::
get_timezone() const
{
  return this->timezone;
}

/* ***************************************************************  */

void libathome_common::Logger::
vprintf(Logger::loglevel_t level, const char* fmt, ::va_list ap)
{
  if (this->loglevel > level) return;

  try {
    RealtimeClock rtc(this->timezone);

    File::open(File::access_t::append_e);

    std::string timestr = rtc.to_string(this->strftime_fmt);
    const char* lvlname = Logger::to_string(level);

    std::string out = timestr + " " + lvlname + ": " + fmt + "\n";
    File::vprintf(out.c_str(), ap);
  } catch (Error& e) {
    /* LOGGER not working here.  So we are using FPRINTF to STDERR for
     * output.
     */
    ::fprintf(stderr, "ERROR: %s\n", e.what());
  }

  File::close();
}

/* ***************************************************************  */

void libathome_common::Logger::
debug(const char* fmt, ...)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(loglevel_t::debug_e, fmt, ap);
  ::va_end(ap);
}

void libathome_common::Logger::
info(const char* fmt, ...)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(loglevel_t::info_e, fmt, ap);
  ::va_end(ap);
}

void libathome_common::Logger::
warn(const char* fmt, ...)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(loglevel_t::warning_e, fmt, ap);
  ::va_end(ap);
}

void libathome_common::Logger::
error(const char* fmt, ...)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(loglevel_t::error_e, fmt, ap);
  ::va_end(ap);
}

void libathome_common::Logger::
fatal(int exit_code, const char* fmt, ...)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(loglevel_t::fatal_e, fmt, ap);
  ::va_end(ap);

  ::exit(exit_code);
}

/* ---------------------------------------------------------------  */

void libathome_common::Logger::
debug(const std::string& fmt, ...)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(loglevel_t::debug_e, fmt.c_str(), ap);
  ::va_end(ap);
}

void libathome_common::Logger::
info(const std::string& fmt, ...)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(loglevel_t::info_e, fmt.c_str(), ap);
  ::va_end(ap);
}

void libathome_common::Logger::
warn(const std::string& fmt, ...)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(loglevel_t::warning_e, fmt.c_str(), ap);
  ::va_end(ap);
}

void libathome_common::Logger::
error(const std::string& fmt, ...)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(loglevel_t::error_e, fmt.c_str(), ap);
  ::va_end(ap);
}

void libathome_common::Logger::
fatal(int exit_code, const std::string& fmt, ...)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(loglevel_t::fatal_e, fmt.c_str(), ap);
  ::va_end(ap);

  ::exit(exit_code);
}

/* ---------------------------------------------------------------  */

void libathome_common::Logger::
debug(const Error& e)
{
  this->debug("%s", e.what());
}

void libathome_common::Logger::
info(const Error& e)
{
  this->info("%s", e.what());
}

void libathome_common::Logger::
warn(const Error& e)
{
  this->warn("%s", e.what());
}

void libathome_common::Logger::
error(const Error& e)
{
  this->error("%s", e.what());
}

void libathome_common::Logger::
fatal(int exit_code, const Error& e)
{
  this->fatal(exit_code, "%s", e.what());
}

/* ***************************************************************  */
