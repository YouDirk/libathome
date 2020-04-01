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


#ifndef LIBATHOME_COMMON_ERROR_H__
#define LIBATHOME_COMMON_ERROR_H__

#include "libathome-common/Common.hpp"

#include <regex>

/* C++ compiler depending stuff
 */
#ifdef __GNUC__
#  /* GNU extension  */
#  define Err(reason ...) \
          libathome_common::Error((__PRETTY_FUNCTION__), reason)
#elif defined OSWIN
#  /* Microsoft extension  */
#  define Err(reason ...) \
          libathome_common::Error((__FUNCSIG__), reason)
#else
#  /* C++ standard  */
#  define Err(reason ...) \
          libathome_common::Error((__func__), reason)
#endif /* ifdef __GNUC__  */


namespace libathome_common
{

/**
 * Base class to `throw` excpetions.
 *
 * Inherit from this class to implement your exception.  So we can
 * `catch (libathome_common::Error& e) {}` all exceptions in one
 * statement.
 *
 * To `throw` this class, use the macro `throw Err("error message!");`
 * defined in libathome-common/Error.hpp.  This makes sure that the
 * `Class::method()` name is included in the Error::what() message.
 *
 * **Example**
 * ```cpp
 * try {
 *
 *   ...
 *
 *   if (error_case)
 *     throw Err("This is an error message!");
 *
 *   ...
 *
 * } catch (Error& e) {
 *   Log->error(e);
 * }
 * ```
 */
class Error: public std::runtime_error
{
public:
  /**
   * Instance it via `throw Err("error message!");`
   *
   * See libathome_common::Error for an example.
   *
   * @param _pretty_func Automatic filled by `Err()` macro
   * @param reason_fmt `printf()`-like error message
   */
  explicit Error(
    const char* _pretty_func, const std::string& reason_fmt, ...);
  /**
   * Instance it via `throw Err("error message!");`
   *
   * See libathome_common::Error for an example.
   *
   * @param _pretty_func Automatic filled by `Err()` macro
   * @param reason_fmt `printf()`-like error message
   */
  explicit Error(const char* _pretty_func, const char* reason_fmt, ...)
    __attribute__ ((format (printf, 3, 4)));

  /**
   * Default destructor.
   */
  virtual ~Error();

  /**
   * Returns the error message which describes the issue which
   * occurs.
   *
   * @return The error message
   */
  virtual const char* what() const noexcept override;

private:
  static const std::regex REGEX_FUNCNAME;
  std::string what_msg;

  void _init(
    const char* _pretty_func, const std::string& reason_fmt, va_list ap);

}; /* class File  */

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_ERROR_H__  */
