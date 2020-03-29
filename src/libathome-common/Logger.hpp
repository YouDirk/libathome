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


#ifndef LIBATHOME_COMMON_LOGGER_H__
#define LIBATHOME_COMMON_LOGGER_H__

#include "libathome-common/Common.hpp"
#include "libathome-common/RealtimeClock.hpp"
#include "libathome-common/File.hpp"
#include "libathome-common/Error.hpp"


namespace libathome_common
{

/**
 * Output class for logging stuff, singleton libathome_common::Log
 *
 * Depending on initialization it logs stuff into log files with a
 * daily log-rotation and deletes old obsolete files from log
 * directory.
 */
class Logger: protected File
{
public:
  /**
   * Default logger instance here: libathome_common::Log.
   *
   * Logger will log to `stdout`.  Will be called during Common::Common.
   */
  explicit Logger();

  /**
   * Default logger instance here: libathome_common::Log.
   *
   * Will be called during Common::Common.
   *
   * @param path Directory where to output log files.
   * @param file_fmt %File name of the log files, use `strftime()`
   *                 formating patterns for representing date/time
   *                 stuff.
   * @param file_count How many files should be kept in `logdir_name`?
   */
  explicit Logger(
    const std::string& path, const std::string& file_fmt,
    unsigned file_count);

  /**
   * Default destructor.
   *
   * Will be called during Common::~Common.
   */
  virtual ~Logger();

  /**
   * Only write log messages which have the represented log-level or a
   * higher priority.
   */
  typedef enum {
    all_e = 0,       ///< Log all possible log messages
    debug_e = 10,    ///< Log: Debug, Infos, Warnings, Errors and Fatals
    info_e = 20,     ///< Log: Infos, Warnings, Errors and Fatals
    warning_e = 30,  ///< Log: Warnings, Errors and Fatals
    error_e = 40,    ///< Log: Errors and Fatals
    fatal_e = 50     ///< Log: Fatals only
  } loglevel_t;

  /**
   * Write debug output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  virtual void debug(const char* fmt, ...)
    const __attribute__ ((format (printf, 2, 3)));
  /**
   * Write info output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  virtual void info(const char* fmt, ...)
    const __attribute__ ((format (printf, 2, 3)));
  /**
   * Write warning output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  virtual void warn(const char* fmt, ...)
    const __attribute__ ((format (printf, 2, 3)));
  /**
   * Write ERROR output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  virtual void error(const char* fmt, ...)
    const __attribute__ ((format (printf, 2, 3)));
  /**
   * Write FATAL ERROR output into log file and `exit()`.
   *
   * @param exit_code Exit code of the terminated process
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  virtual void fatal(int exit_code, const char* fmt, ...)
    const __attribute__ ((format (printf, 3, 4)));

  /**
   * Write runtime error as debug output into log file.
   *
   * @param e Runime error (exception) to output
   */
  virtual void debug(const Error& e) const;
  /**
   * Write runtime error as info output into log file.
   *
   * @param e Runime error (exception) to output
   */
  virtual void info(const Error& e) const;
  /**
   * Write runtime error as warning output into log file.
   *
   * @param e Runime error (exception) to output
   */
  virtual void warn(const Error& e) const;
  /**
   * Write runtime error as ERROR output into log file.
   *
   * @param e Runime error (exception) to output
   */
  virtual void error(const Error& e) const;
  /**
   * Write runtime error as FATAL ERROR output into log file.
   *
   * @param exit_code Exit code of the terminated process
   * @param e Runime error (exception) to output
   */
  virtual void fatal(int exit_code, const Error& e) const;

private:
  std::string file_fmt;
  unsigned file_count;

  Logger::loglevel_t loglevel;
  RealtimeClock::timezone_t timezone;
  std::string strftime_fmt;

  void _init();

  void _printf(
    Logger::loglevel_t level, const std::string& name, const char* fmt,
    va_list ap) const;

}; /* class Logger  */

/**
 * Reference to the singleton class Logger instance.
 *
 * Will be instanciated during Common::Common and freed during
 * Common::~Common().
 */
extern Logger* Log;

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_LOGGER_H__  */
