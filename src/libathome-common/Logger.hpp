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
};

/**
 * Reference to the singleton class Logger instance.
 *
 * Will be instanciated during Common::Common and freed during
 * Common::~Common().
 */
extern Logger* Log;

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_LOG_H__  */
