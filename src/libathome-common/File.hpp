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


#ifndef LIBATHOME_COMMON_FILE_H__
#define LIBATHOME_COMMON_FILE_H__
/**
 * @file
 * @brief Declares the class libathome_common::File.
 */

#include "libathome-common/Common.hpp"

namespace libathome_common
{

/**
 * Base class for file I/O.
 *
 * Inherit from this class to implement special cases, such like
 * libathome_common::Logger or libathome_common::ConfigFile.  After
 * construction no file access operation will be done.  So you can
 * safe use instances of this class as member variables.
 *
 * After constructed you need to open the file with File::open().
 * Then you can write to it with File::print(), File::printf() or read
 * with File::scanf().  If you finished then File::close() the file.
 */
class File
{
public:

  /**
   * The access mode of the opened file.
   *
   * Pass it via File::open().
   */
  typedef enum {
    read_e = 0,    ///< Open file for read
    write_e = 1,   ///< Open file for write; will be created if not exist.
    append_e = 2   ///< Append to file; will be created if not exist.
  } access_t;

  /**
   * Convert a File::access_t to string.
   *
   * @param access The access mode to convert
   * @return The string which names the access mode. `static`
   *         allocated, need NOT to be `free()`d.
   */
  static const char* to_string(File::access_t access);

  /**
   * Construct a file with an externally opened `fstream`.
   *
   * The calls of File::open() and File::close() will be ignored.  Can
   * be safe used with `stdout`, `stdin` and `stderr`.
   *
   * @param fstream The externally managed filestream, such like
   *                `stdout`, `stdin` and `stderr`
   * @param stream_name A human readable name of the `fstream`, such
   *                    like `"<stdout>"`
   * @exception Error will be thrown if `fstream` was set to `NULL`
   */
  explicit File(::FILE* fstream, const std::string& stream_name)
    noexcept(false);
  /**
   * Construct a file with real filename on filesystem.
   *
   * It just setup all needed information for file operations but
   * nothing will be done until File::open() was called.  The
   * directory of `path` will be created on File::open().  Parameter
   * `binary` can be set to `false` to translate automatically `"\n"`
   * characters to Windows newlines `"\r\n"` on such systems.
   *
   * @param path The path to the file.  The directory will be created
   *             on File::open()
   * @param filename The filename of the file
   * @param binary Set to `false` to translate automatically `"\n"`
   *               characters to Windows newlines `"\r\n"` on such
   *               systems
   */
  explicit File(
    const std::string& path, const std::string& filename, bool binary);
  /**
   * Default destructor.
   */
  virtual ~File();

  /**
   * Open file for File::read_e, File::write_e or File::append_e.
   *
   * Call this before writing or reading from file.  Depending on
   * `mode` the file will be created.  See File::access_t for more
   * details.
   *
   * @param mode Mode for opening, likes read/write/append
   * @exception Error will be thrown if file could not be opened
   */
  virtual void open(File::access_t mode) noexcept(false);
  /**
   * Close the file and write to disk or unlock to write.
   *
   * If opened for File::write_e or File::append_e then on close the
   * file will be written to disk.  If File::read_e then closing will
   * make other file descriptors available to write to the file.
   */
  virtual void close();

  /**
   * Write string to file.
   *
   * @param fmt `printf()`-like format string
   * @param ... Arguments of `fmt` string
   * @exception Error will be thrown if writing to file has failed
   */
  virtual void printf(const char* fmt, ...) const noexcept(false)
    __attribute__((format (printf, 2, 3)));
  /**
   * Write string to file.
   *
   * @param output String which should be logged
   * @exception Error will be thrown if writing to file has failed
   */
  virtual void print(const std::string& output) const noexcept(false);

protected:
  /**
   * Use this method to output to file if you inherit from this class
   * and implement your own File class.
   *
   * @param fmt `printf()`-like format string
   * @param ap Arguments of `fmt` string as `va_list`
   */
  virtual void vprintf(const char* fmt, ::va_list ap)
    const noexcept(false);

private:
  ::FILE* extern_fstream;

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
  ::FILE* fstream;
  /**
   * Undefined if `File::fstream == NULL` (file closed).
   */
  File::access_t mode;

}; /* class File  */

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_FILE_H__  */
