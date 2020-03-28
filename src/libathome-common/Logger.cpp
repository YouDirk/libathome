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

#include <ctime>  /* Same as <stdarg.h>  */

libathome_common::Logger::
Logger(
  const std::string& logdir_name, const std::string& logfile_fmt,
  unsigned logcount_delete)
  :logdir_name(logdir_name), logfile_fmt(logfile_fmt),
   logcount_delete(logcount_delete), loglevel(log_level_t::all_e),
   timezone(timezone_t::local_e), strftime_fmt("[%T]")
{
  if (this->logdir_name.empty()) {
    this->fstream = stdout;
  } else {
    this->fstream = NULL;
  }
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
  Logger::log_level_t level, const std::string& name,
  const std::string& fmt, va_list ap) const
{
  if (this->loglevel > level) return;

  time_t timestamp;
  if (0 > time(&timestamp)) {
    printf("warning: Logger: Could not get time from RTC!\n");
    timestamp = (time_t) 0;
  }

  tm timestruct;
  tm* timestruct_result = NULL;
  switch (this->timezone) {
  case utc_e:
    timestruct_result = gmtime_r(&timestamp, &timestruct);
    break;
  case local_e:
    timestruct_result = localtime_r(&timestamp, &timestruct);
    break;
  }
  if (timestruct_result != &timestruct) {
    printf("ERROR: Logger: Could not convert unix timestamp to struct!\n");
    return;
  }

  string_t timestr;
  if (0 == strftime(timestr, STRING_LEN, strftime_fmt.c_str(),
                    &timestruct)) {
    printf("ERROR: Logger: Could not convert time struct to string!\n");
    return;
  }

  FILE* fs = this->_open();
  if (fs == NULL) return;

  std::string out;
  out.reserve(STRING_LEN + name.size() + fmt.size() + 10);
  out = timestr + (" " + name + ": " + fmt + "\n");

  if (0 == vfprintf(fs, out.c_str(), ap)) {
    printf("ERROR: Logger: Could not write '%s' message!\n", name);
    return;
  }

  this->_close(fs);
}

/* ***************************************************************  */

void libathome_common::Logger::
debug(const std::string& fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(log_level_t::debug_e, "debug", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
info(const std::string& fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(log_level_t::info_e, "info", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
warn(const std::string& fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(log_level_t::warning_e, "warning", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
error(const std::string& fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(log_level_t::error_e, "ERROR", fmt, ap);
  va_end(ap);
}

void libathome_common::Logger::
fatal(int exit_code, const std::string& fmt, ...) const
{
  va_list ap;

  va_start(ap, fmt);
  this->_printf(log_level_t::fatal_e, "FATAL", fmt, ap);
  va_end(ap);

  exit(exit_code);
}
