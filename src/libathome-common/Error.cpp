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


#include "libathome-common/Error.hpp"

#ifdef __GNUC__
#  /* For backtrace stuff  */
#  include <execinfo.h>
#endif /* ifdef __GNUC__  */


const std::regex libathome_common::Error::
REGEX_FUNCNAME("^(.* )?([^ (]*)\\(.*$");

/* ---------------------------------------------------------------  */

void libathome_common::Error::
_init(const char* _pretty_func, const std::string& reason_fmt,
      va_list ap)
{
#ifdef __GNUC__
  this->backtrace_size
    = backtrace(this->backtrace_frames, Error::BACKTRACE_MAX);
  this->backtrace_symbolz
    = backtrace_symbols(this->backtrace_frames, this->backtrace_size);
#else /* ifdef __GNUC__  */
  this->backtrace_size = 0;
  this->backtrace_frames[0] = NULL;
  this->backtrace_symbolz = NULL;
#endif /* ifdef __GNUC__  */

  /* ---  */

  std::string func = _pretty_func != NULL
    ? std::regex_replace(_pretty_func, Error::REGEX_FUNCNAME, "$2")
    : "";

  if (func.empty())
    func = _pretty_func != NULL? _pretty_func: "???";

  string_t buf;
  if (0 ==
      vsnprintf(buf, STRING_LEN, reason_fmt.c_str(), ap)) {
    /* Throwing Error in Error is a bad idea.  So we are making the
     * best what is possible.
     */
    strncpy(buf, reason_fmt.c_str(), STRING_LEN);
  }

  this->what_msg.reserve(func.size() + strlen(buf) + 40);
  this->what_msg = "*(RUNTIME)* " + func + "(): " + buf;

  /* ---  */
  /* Only in DEBUG build output backtrace in WHAT()  */

#ifdef DEBUG
  this->what_msg += "\n\nbacktrace:";

  if (this->backtrace_size == 0)
    this->what_msg += "\n  <Not implemented for your compiler or OS>";

  int output_size = this->get_backtrace_size();
  for (int i=0; i<output_size; i++) {
    if (0 == snprintf(buf, STRING_LEN, "\n  %s",
                      backtrace_symbolz[i + Error::BACKTRACE_OFFSET])) {
      /* Throwing Error in Error is a bad idea.  So we are making the
       * best what is possible.
       */
      continue;
    }
    this->what_msg += buf;
  }

  this->what_msg += this->is_backtrace_more()
    ? "\n      (...)\n"
    : "\n";
#endif /* ifdef DEBUG  */

}

libathome_common::Error::
Error(const char* _pretty_func, const std::string& reason_fmt, ...)
  :std::runtime_error(reason_fmt)
{
  va_list ap;

  va_start(ap, reason_fmt);
  this->_init(_pretty_func, reason_fmt, ap);
  va_end(ap);
}

libathome_common::Error::
Error(const char* _pretty_func, const char* reason_fmt, ...)
  :std::runtime_error(std::string(reason_fmt))
{
  va_list ap;

  va_start(ap, reason_fmt);
  this->_init(_pretty_func, std::string(reason_fmt), ap);
  va_end(ap);
}

libathome_common::Error::
~Error()
{
  if (this->backtrace_symbolz) free(this->backtrace_symbolz);
}

/* ---------------------------------------------------------------  */

const char* libathome_common::Error::
what() const noexcept
{
  return this->what_msg.c_str();
}

int libathome_common::Error::
get_backtrace_size()
{
  int result = this->backtrace_size - Error::BACKTRACE_OFFSET;

  return result < 0? 0: result;
}
bool libathome_common::Error::
is_backtrace_more()
{
  return this->backtrace_size == Error::BACKTRACE_MAX;
}
const void* libathome_common::Error::
get_backtrace_frame(int i)
{
  /* Don't THROW, because an Error in Error is a bad idea o.o  */
  if (i < 0 || i >= this->backtrace_size) return NULL;

  return backtrace_frames[i + Error::BACKTRACE_OFFSET];
}
const char* libathome_common::Error::
get_backtrace_symbol(int i)
{
  /* Don't THROW, because an Error in Error is a bad idea o.o  */
  if (i < 0 || i >= this->backtrace_size) return NULL;

  return backtrace_symbolz[i + Error::BACKTRACE_OFFSET];
}
