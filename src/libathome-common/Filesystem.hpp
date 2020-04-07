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


#ifndef LIBATHOME_COMMON_FILESYSTEM_H__
#define LIBATHOME_COMMON_FILESYSTEM_H__

#include "libathome-common/Common.hpp"

namespace libathome_common
{

/**
 * Abstract class for OS dependent filesystem operations.
 *
 * This class collects some static operating system dependent methods
 * and constants for filesystem operations.
 */
class Filesystem
{
public:
  /**
   * OS dependent path seperator between directories.
   *
   * Usally on Linux/Unix-like systems it is set to `"/"`, on
   * MSYS/MinGW/Windows it is set to `"\\"`.
   */
  static const char* PATH_SEPERATOR;
  /**
   * OS dependent relativ path which points to the current directory.
   *
   * Usally set to `"."`.
   */
  static const char* PATH_DOT;
  /**
   * OS dependent relativ path which points to the parent directory.
   *
   * Usally set to `".."`.
   */
  static const char* PATH_DOTDOT;

  /**
   * Creates a directory in `path`, **not recursively**.
   *
   * Creates a directory in `path` **not recursively** and returns
   * `true` on success.  If the directory already exist then `false`
   * will be returned.  Throws an libathome_common::Error if an error
   * occurs.
   *
   * @param path The path (relative or absolute) where to create the
   *             directory.
   * @return `true` on success, `false` if directory does already
   *         exist.
   * @exception Error will be thrown if an error occurs, such like no
   *            permission, etc.
   */
  static bool mkdir(const std::string& path) noexcept(false);

private:
  /**
   * Mask for default `umode` of files/directories.
   *
   * The `umode` is calculated as `umode & ~umask & 0777`.  Means if
   * `umode == 0777` then the system default file access will be set.
   * `umask` is set by user or administrator.
   */
  static const unsigned _UMODE_DEFAUL;

  virtual void _abstract_class() = 0;
}; /* class Filesystem  */

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_FILESYSTEM_H__  */
