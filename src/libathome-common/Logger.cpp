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


libathome_common::Logger::
Logger()
  :fstream(stdout), logdir_name("log"), logfile_fmt("%Y-%m-%d.log"),
   logcount_delete(365), loglevel(log_level_t::all_e),
   timezone(RealtimeClock::timezone_t::local_e),
   strftime_fmt("[%H:%M:%S]")
{
}

libathome_common::Logger::
Logger(
  const std::string& logdir_name, const std::string& logfile_fmt,
  unsigned logcount_delete)
  :Logger()
{
  this->fstream = NULL;
  this->logdir_name = logdir_name;
  this->logfile_fmt = logfile_fmt;
  this->logcount_delete = logcount_delete;
}

libathome_common::Logger::
~Logger()
{

}

libathome_common::Logger* libathome_common::Log = NULL;

/* ***************************************************************  */

FILE* libathome_common::Logger::
_open() const
{
  if (this->fstream != NULL) return this->fstream;

  FILE* result;
  // TODO

  return result;
}

void libathome_common::Logger::
_close(FILE* fs) const
{
}

void libathome_common::Logger::
_printf(
  Logger::log_level_t level, const std::string& name, const char* fmt,
  va_list ap) const
{
  if (this->loglevel > level) return;

  RealtimeClock rtc(timezone);

  FILE* fs = this->_open();
  if (fs == NULL) return;

  std::string timestr;
  rtc.to_string(timestr, strftime_fmt);

  std::string out;
  out.reserve(timestr.size() + name.size() + strlen(fmt) + 10);
  out = timestr + " " + name + ": " + fmt + "\n";

  if (0 == vfprintf(fs, out.c_str(), ap)) {
    fprintf(stderr, "ERROR: Logger: Could not write '%s' message!\n",
            name.c_str());
    return;
  }

  this->_close(fs);
}

/* ***************************************************************  */

void libathome_common::Logger::
debug(const char* fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(log_level_t::debug_e, "debug", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
info(const char* fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(log_level_t::info_e, "info", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
warn(const char* fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(log_level_t::warning_e, "warning", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
error(const char* fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(log_level_t::error_e, "ERROR", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
fatal(int exit_code, const char* fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(log_level_t::fatal_e, "FATAL", fmt, ap);
  va_end(ap);

  exit(exit_code);
}
