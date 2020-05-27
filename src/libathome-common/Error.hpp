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


#ifndef LIBATHOME_COMMON_ERROR_H__
#define LIBATHOME_COMMON_ERROR_H__
/**
 * @file
 * @brief Declares the class libathome_common::Error.
 */

#include "libathome-common/Common.hpp"

#include <regex>

/* Debugging stuff
 */

/// @cond Doxygen_Suppress
#ifdef DEBUG
#define _ERROR_BT                  true
#else
#define _ERROR_BT                  false
#endif /* ifdef DEBUG  */
/// @endcond

/* C++ compiler depending stuff
 */

/**
 * **Constructor** macro to `throw` an libathome_common::Error.
 *
 * It automatically provides some parameters to
 * libathome_common::Error::Error() so that the
 * libathome_common::Error::what() message includes the **method
 * name** of the throwing method and optionally (depending on `DEBUG`)
 * a **backtrace**.
 *
 * **Example**
 * ```cpp
 * try {
 *
 *   ...
 *
 *   if (error_case)
 *     throw Err("This is an %s message!", "error");
 *
 *   ...
 *
 * } catch (Error& e) {
 *   Log->error(e);
 * }
 * ```
 */
#ifdef __GNUC__
#  /* GNU extension  */
#  define Err(reason ...) libathome_common::Error( \
          _ERROR_BT, (__PRETTY_FUNCTION__), reason)
#elif defined OSWIN
#  /* Microsoft extension  */
#  define Err(reason ...) libathome_common::Error( \
          _ERROR_BT, (__FUNCSIG__), reason)
#else
#  /* C++ standard  */
#  define Err(reason ...) libathome_common::Error( \
          _ERROR_BT, (__func__), reason)
#endif /* ifdef __GNUC__  */


namespace libathome_common
{

/**
 * Base class to `throw` excpetions, use ::Err() macro to `throw`.
 *
 * Inherit from this class to implement your exception.  So we can
 * `catch (libathome_common::Error& e) {}` all exceptions in one
 * statement.
 *
 * To `throw` this class, use the ::Err() macro `throw Err("error
 * message!");` defined in libathome-common/Error.hpp.  This makes
 * sure that the `Class::method()` name is included in the
 * Error::what() message.
 *
 * This class does also provides a `backtrace` functionality.  The
 * backtrace will be automatically added to Error::what(), if debug
 * build is enabled by defining the preprocessor constant `DEBUG`,
 * i.e. passing the flag `-DDEBUG` through the C++ compiler.
 *
 * **Example**
 * ```cpp
 * try {
 *
 *   ...
 *
 *   if (error_case)
 *     throw ::Err("This is an %s message!", "error");
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
   * Instance it via `throw ::Err("error %s!", "message");`
   *
   * See libathome_common::Error for an example.
   *
   * @param _backtrace_append Automatically filled by `::Err()` macro
   * @param _pretty_func Automatically filled by `::Err()` macro
   * @param reason_fmt `printf()`-like error message
   */
  explicit Error(bool _backtrace_append, const char* _pretty_func,
    const std::string& reason_fmt, ...);
  /**
   * Instance it via `throw ::Err("error %s!", "message");`
   *
   * See libathome_common::Error for an example.
   *
   * @param _backtrace_append Automatically filled by `::Err()` macro
   * @param _pretty_func Automatically filled by `::Err()` macro
   * @param reason_fmt `printf()`-like error message
   */
  explicit Error(bool _backtrace_append, const char* _pretty_func,
    const char* reason_fmt, ...) __attribute__((format (printf, 4, 5)));

  /**
   * Default destructor.
   */
  virtual ~Error();

  /**
   * Append backtrace to Error::what().
   *
   * Also append backtrace to Error::what() if `-DDEBUG` was not set.
   * Double calls will be ignored.
   */
  virtual void bt() noexcept;

  /**
   * Returns the error message which describes the issue which
   * occurs.
   *
   * @return The error message
   */
  virtual const char* what() const noexcept override;

  /**
   * Use this size to iterate through the backtrace.
   *
   * @return Size of backtrace or `0` if not generated
   */
  virtual int get_backtrace_size() const noexcept;
  /**
   * Returns if the real stack was bigger than
   * Error::get_backtrace_size().
   *
   * @return `true` if real stack was bigger than
   *         Error::get_backtrace_size()
   */
  virtual bool is_backtrace_more() const noexcept;
  /**
   * Returns a pointer to the `i`-th stack frame of the backtrace.
   *
   * @param i Index of the stack frame you want to get
   * @return Pointer of the `i`-th stack frame.  Or `NULL` if `i` is
   *         out of range.
   */
  virtual const void* get_backtrace_frame(int i) const noexcept;
  /**
   * Returns a symbolic representation of the `i`-th stack frame of
   * the backtrace.
   *
   * @param i Index of the stack frame you want to get
   * @return Symbolic representation of the `i`-th stack frame.  Or
   *         `NULL` if `i` is out of range.
   */
  virtual const char* get_backtrace_symbol(int i) const noexcept;

private:
  static const std::regex REGEX_FUNCNAME;
  static const std::regex REGEX_LIBNAME;

  std::string what_msg;
  bool backtrace_appended;

  static const int BACKTRACE_MAX = 23;
  /**
   * The first 3 frames are Error::_backtrace(), Error::_init() and
   * Error::Error() which we don´t want to see in backtrace.
   */
  static const int BACKTRACE_OFFSET = 3;
  void* backtrace_frames[BACKTRACE_MAX];
  char** backtrace_symbolz;
  int backtrace_size;

  /**
   * `__attribute__((noinline))` makes sure that
   * Error::BACKTRACE_OFFSET is set correct.
   */
  void _init(bool _backtrace_append,
    const char* _pretty_func, const std::string& reason_fmt, va_list ap)
    __attribute__((noinline));

  /**
   * `__attribute__((noinline))` makes sure that
   * Error::BACKTRACE_OFFSET is set correct.
   */
  int _backtrace(void** buffer, int size) __attribute__((noinline));

  /**
   * The result must be `free()`, usally in destructor `~Error()`!
   *
   * @return Is allocated with `malloc()` and mus be `free()` by
   *         caller.  Or NULL if not implemented.
   */
  char** _backtrace_symbols(void* const* buffer, int size);

}; /* class Error  */

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_ERROR_H__  */
