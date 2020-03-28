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


#ifndef LIBATHOME_COMMON_LOG_H__
#define LIBATHOME_COMMON_LOG_H__

#include "libathome-common/Common.hpp"

namespace libathome_common
{

/**
 * Output class for logging stuff, singleton libathome_common::Log
 *
 * Depending on initialization it logs stuff into log files with a
 * daily log-rotation and deletes old obsolete files from log
 * directory.
 */
class Logger
{
public:
  /**
   * DO NOT CONSTRUCT, use libathome_common::Log instead!
   *
   * Will be called during Common::Common.
   *
   * @param logdir_name If `""` then output to `stdout`, otherwise
   *                    directory where to output log files.
   *
   * @param logfile_fmt File name of the log files, use `strftime()`
   *                    formating patterns for representing date/time
   *                    stuff.
   *
   * @param logcount_delete How many files should be kept in
   *                        `logdir_name`?
   */
  explicit Logger(
    const std::string& logdir_name, const std::string& logfile_fmt,
    unsigned logcount_delete);
  /**
   * DO NOT DESTRUCT, use libathome_common::Log instead!
   *
   *Will be called during Common::~Common.
   */
  virtual ~Logger();

  typedef enum {
    all_e = 0, debug_e = 10, info_e = 20, warning_e = 30, error_e = 40,
    fatal_e = 50
  } log_level_t;

  typedef enum {
    utc_e = 0, local_e = 1
  } timezone_t;

  /** Write debug output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  void debug(const char* fmt, ...)
    const __attribute__ ((format (printf, 2, 3)));
  /** Write info output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  void info(const char* fmt, ...)
    const __attribute__ ((format (printf, 2, 3)));
  /** Write warning output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  void warn(const char* fmt, ...)
    const __attribute__ ((format (printf, 2, 3)));
  /** Write ERROR output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  void error(const char* fmt, ...)
    const __attribute__ ((format (printf, 2, 3)));
  /** Write FATAL ERROR output into log file and `exit()`.
   *
   * @param exit_code Exit code of the terminated process
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  void fatal(int exit_code, const char* fmt, ...)
    const __attribute__ ((format (printf, 3, 4)));

private:
  const std::string logdir_name;
  const std::string logfile_fmt;
  unsigned logcount_delete;

  Logger::log_level_t loglevel;
  Logger::timezone_t timezone;
  std::string strftime_fmt;
  FILE* fstream;

  void _printf(
    Logger::log_level_t level, const std::string& name, const char* fmt,
    va_list ap) const;

  FILE* _open() const;
  void _close(FILE* fs) const;

}; /* class Logger  */

/**
 * Reference to the singleton class Logger instance.
 *
 * Will be instanciated during Common::Common and freed during
 * Common::~Common().
 */
extern Logger* Log;

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_LOG_H__  */
