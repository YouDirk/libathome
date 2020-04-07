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


#ifndef LIBATHOME_COMMON_FILE_H__
#define LIBATHOME_COMMON_FILE_H__

#include "libathome-common/Common.hpp"

namespace libathome_common
{

/**
 * Base class for file I/O.
 *
 * Inherit from this class to implement special cases, such like
 * libathome_common::Logger or libathome_common::FileConfig.  The
 * `path` of the file and the `fie` itself will be created during
 * File::open_write() or File::open_append().
 */
class File
{
public:
  static const char* PATH_SEPERATOR;
  static const char* PATH_DOT;
  static const char* PATH_DOTDOT;

  typedef enum {
    read_e = 0,
    write_e = 1,
    append_e = 2
  } access_t;

  explicit File(FILE* fstream, const std::string& stream_name)
    noexcept(false);
  explicit File(
    const std::string& path, const std::string& filename, bool binary);
  virtual ~File();

  virtual void open(File::access_t mode);
  virtual void close();

protected:
  virtual void vprintf(const std::string& fmt, va_list ap) const;

private:
  FILE* extern_fstream;

  bool binary;
  std::string path;
  std::string filename;

  /**
   * Full filename or stream name.
   *
   * Full filename such like *'path/to/file'* if `extern_fstream ==
   * NULL`.  Otherwise it is set to name of File::extern_fstream .
   */
  std::string filename_full;

  /**
   * If file is closed then `File::fstream == NULL`.
   */
  FILE* fstream;
  /**
   * Undefined if `File::fstream == NULL` (file closed).
   */
  File::access_t mode;

}; /* class File  */

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_FILE_H__  */
