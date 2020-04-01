/* lib@home, framework to develop distributed calculations.
 * Copyright (C) 2020  Dirk "YouDirk" Lehmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "libathome-common/Logger.hpp"


void libathome_common::Logger::
_init()
{
  this->loglevel = loglevel_t::all_e;
  this->timezone = RealtimeClock::timezone_t::local_e;
  this->strftime_fmt = "[%H:%M:%S]";
}

libathome_common::Logger::
Logger()
  :File(stdout), file_fmt("%Y-%m-%d.log"), file_count(365)
{
  this->_init();
}

libathome_common::Logger::
Logger(
  const std::string& path, const std::string& file_fmt,
  unsigned file_count)
  :File(path, "tmp.log", false), file_fmt(file_fmt),
   file_count(file_count)
{
  this->_init();
}

libathome_common::Logger::
~Logger()
{
}

libathome_common::Logger* libathome_common::Log = NULL;

/* ***************************************************************  */

void libathome_common::Logger::
_printf(
  Logger::loglevel_t level, const std::string& lvlname, const char* fmt,
  va_list ap) const
{
  if (this->loglevel > level) return;

  try {
    RealtimeClock rtc(timezone);

    /*
      FILE* fs = this->_open();
      if (fs == NULL) return;
    */
    FILE* fs = stdout;

    std::string timestr;
    rtc.to_string(timestr, strftime_fmt);

    std::string out;
    out.reserve(timestr.size() + lvlname.size() + strlen(fmt) + 10);
    out = timestr + " " + lvlname + ": " + fmt + "\n";

    if (0 == vfprintf(fs, out.c_str(), ap))
      throw Err("Could not write '%s' message!", lvlname.c_str());

    //this->_close(fs);
  } catch (Error& e) {
    /* LOGGER not working here.  So we are using FPRINTF to STDERR for
     * output.
     */
    fprintf(stderr, "ERROR: %s\n", e.what());
  }
}

/* ***************************************************************  */

void libathome_common::Logger::
debug(const char* fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(loglevel_t::debug_e, "debug", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
info(const char* fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(loglevel_t::info_e, "info", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
warn(const char* fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(loglevel_t::warning_e, "warning", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
error(const char* fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(loglevel_t::error_e, "ERROR", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
fatal(int exit_code, const char* fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(loglevel_t::fatal_e, "FATAL", fmt, ap);
  va_end(ap);

  exit(exit_code);
}

/* ---------------------------------------------------------------  */

void libathome_common::Logger::
debug(const std::string& fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(loglevel_t::debug_e, "debug", fmt.c_str(), ap);
  va_end(ap);
}

void libathome_common::Logger::
info(const std::string& fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(loglevel_t::info_e, "info", fmt.c_str(), ap);
  va_end(ap);
}

void libathome_common::Logger::
warn(const std::string& fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(loglevel_t::warning_e, "warning", fmt.c_str(), ap);
  va_end(ap);
}

void libathome_common::Logger::
error(const std::string& fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(loglevel_t::error_e, "ERROR", fmt.c_str(), ap);
  va_end(ap);
}

void libathome_common::Logger::
fatal(int exit_code, const std::string& fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(loglevel_t::fatal_e, "FATAL", fmt.c_str(), ap);
  va_end(ap);

  exit(exit_code);
}

/* ---------------------------------------------------------------  */

void libathome_common::Logger::
debug(const Error& e) const
{
  this->debug(e.what());
}

void libathome_common::Logger::
info(const Error& e) const
{
  this->info(e.what());
}

void libathome_common::Logger::
warn(const Error& e) const
{
  this->warn(e.what());
}

void libathome_common::Logger::
error(const Error& e) const
{
  this->error(e.what());
}

void libathome_common::Logger::
fatal(int exit_code, const Error& e) const
{
  this->fatal(exit_code, e.what());
}

/* ***************************************************************  */
