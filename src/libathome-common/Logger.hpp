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


#ifndef LIBATHOME_COMMON_LOGGER_H__
#define LIBATHOME_COMMON_LOGGER_H__
/**
 * @file
 * @brief Declares the class ::libathome_common::Logger.
 */

#include "libathome-common/Common.hpp"
#include "libathome-common/RealtimeClock.hpp"
#include "libathome-common/File.hpp"
#include "libathome-common/Error.hpp"


namespace libathome_common
{

/**
 * Output class for logging stuff, singleton ::libathome_common::Log
 *
 * Depending on initialization it logs stuff into log files with a
 * daily log-rotation and deletes old obsolete files from log
 * directory.
 */
class Logger: protected File
{
public:

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
    fatal_e = 50,    ///< Log: Fatals only
    none_e = 60      ///< Log: Nothing, prevent logging
  } loglevel_t;

  /**
   * Convert a ::libathome_common::Logger::loglevel_t to string.
   *
   * @param loglevel The loglevel to convert
   * @return The string which names the loglevel. `static` allocated,
   *         need NOT to be `free()`d.
   */
  static const char* to_string(Logger::loglevel_t loglevel);

  /**
   * Default logger instance here: ::libathome_common::Log.
   *
   * Will be log to `stdout` and be called during
   * ::libathome_common::Common::Common().
   *
   * @param loglevel Log-level to filter by priority
   * @param timezone Timezone which is used for messages
   */
  explicit Logger(
    Logger::loglevel_t loglevel, RealtimeClock::timezone_t timezone);

  /**
   * Default logger instance here: ::libathome_common::Log.
   *
   * Will be called during ::libathome_common::Common::Common().
   *
   * @param loglevel Log-level to filter by priority
   * @param timezone Timezone which is used for messages
   * @param path Directory where to output log files.
   * @param file_fmt %File name of the log files, use `strftime()`
   *                 formating patterns for representing date/time
   *                 stuff.
   * @param file_count How many files should be kept in `logdir_name`?
   */
  explicit Logger(
    Logger::loglevel_t loglevel, RealtimeClock::timezone_t timezone,
    const std::string& path, const std::string& file_fmt,
    unsigned file_count);

  /**
   * Default destructor.
   *
   * Will be called during ::libathome_common::Common::~Common.
   */
  virtual ~Logger();

  /**
   * Set the ::libathome_common::Logger::loglevel_t of the Logger.
   *
   * Set the log-level to filter out messages which we don't want to
   * see.
   *
   * @param loglevel The log-level to set
   */
  virtual void set_loglevel(Logger::loglevel_t loglevel);
  /**
   * Set the ::libathome_common::RealtimeClock::timezone_t of the
   * Logger.
   *
   * Set the timezone of the timestamps for the log messages.
   * ::libathome_common::RealtimeClock::utc_e is usefull for
   * server-side parts.  On client side it's better to use the local
   * system time ::libathome_common::RealtimeClock::local_e.
   *
   * @param timezone The timezone to set
   */
  virtual void set_timezone(RealtimeClock::timezone_t timezone);

  /**
   * Returns the ::libathome_common::Logger::loglevel_t of the Logger.
   *
   * See ::libathome_common::Logger::set_loglevel for more details.
   *
   * @return The current log-level
   */
  virtual Logger::loglevel_t get_loglevel() const;
  /**
   * Returns the ::libathome_common::RealtimeClock::timezone_t of the
   * Logger.
   *
   * See ::libathome_common::Logger::set_timezone for more details.
   *
   * @return The current timezone
   */
  virtual RealtimeClock::timezone_t get_timezone() const;

  /* -------------------------------------------------------------  */

  /**
   * Write debug output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  virtual void debug(const char* fmt, ...)
    __attribute__((format (printf, 2, 3)));
  /**
   * Write info output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  virtual void info(const char* fmt, ...)
    __attribute__((format (printf, 2, 3)));
  /**
   * Write warning output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  virtual void warn(const char* fmt, ...)
    __attribute__((format (printf, 2, 3)));
  /**
   * Write ERROR output into log file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  virtual void error(const char* fmt, ...)
    __attribute__((format (printf, 2, 3)));
  /**
   * Write FATAL ERROR output into log file and `exit()`.
   *
   * @param exit_code Exit code of the terminated process
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   */
  virtual void fatal(int exit_code, const char* fmt, ...)
    __attribute__((format (printf, 3, 4)));

  /* -------------------------------------------------------------  */

  /**
   * Write debug output into log file.
   *
   * @param output String which should be logged
   */
  virtual void debug(const std::string& output);
  /**
   * Write info output into log file.
   *
   * @param output String which should be logged
   */
  virtual void info(const std::string& output);
  /**
   * Write warning output into log file.
   *
   * @param output String which should be logged
   */
  virtual void warn(const std::string& output);
  /**
   * Write ERROR output into log file.
   *
   * @param output String which should be logged
   */
  virtual void error(const std::string& output);
  /**
   * Write FATAL ERROR output into log file and `exit()`.
   *
   * @param exit_code Exit code of the terminated process
   * @param output String which should be logged
   */
  virtual void fatal(int exit_code, const std::string& output);

  /* -------------------------------------------------------------  */

  /**
   * Write runtime error as debug output into log file.
   *
   * @param e Runime error (exception) to output
   */
  virtual void debug(const Error& e);
  /**
   * Write runtime error as info output into log file.
   *
   * @param e Runime error (exception) to output
   */
  virtual void info(const Error& e);
  /**
   * Write runtime error as warning output into log file.
   *
   * @param e Runime error (exception) to output
   */
  virtual void warn(const Error& e);
  /**
   * Write runtime error as ERROR output into log file.
   *
   * @param e Runime error (exception) to output
   */
  virtual void error(const Error& e);
  /**
   * Write runtime error as FATAL ERROR output into log file.
   *
   * @param exit_code Exit code of the terminated process
   * @param e Runime error (exception) to output
   */
  virtual void fatal(int exit_code, const Error& e);

  /* -------------------------------------------------------------  */
protected:
  /**
   * Use this method to output to log-file if you inherit from this
   * class and implement your own ::libathome_common::Logger class.
   *
   * @param level The loglevel which will be used.
   * @param fmt `printf()`-like format string
   * @param ap Arguments of `fmt` string as `va_list`
   */
  void vprintf(Logger::loglevel_t level, const char* fmt, ::va_list ap);

private:
  Logger::loglevel_t loglevel;
  RealtimeClock::timezone_t timezone;

  std::string file_fmt;
  unsigned file_count;

  std::string strftime_fmt;

  void _init();
}; /* class Logger  */

/**
 * Reference to the singleton class ::libathome_common::Logger
 * instance.
 *
 * Will be instanciated during ::libathome_common::Common::Common and
 * freed during ::libathome_common::Common::~Common().
 */
extern Logger* Log;

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_LOGGER_H__  */
