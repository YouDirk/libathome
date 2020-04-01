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


const std::regex libathome_common::Error::
REGEX_FUNCNAME("^(.* )?([^ (]*)\\(.*$");

void libathome_common::Error::
_init(const char* _pretty_func, const std::string& reason_fmt,
      va_list ap)
{
  std::string func = _pretty_func != NULL
    ? std::regex_replace(_pretty_func, Error::REGEX_FUNCNAME, "$2")
    : "";

  if (func.empty())
    func = _pretty_func != NULL? _pretty_func: "???";

  string_t reason_filled;
  if (0 ==
      vsnprintf(reason_filled, STRING_LEN, reason_fmt.c_str(), ap)) {
    /* Throwing Error in Error is a bad idea.  So we are making the
       best what is possible.
     */
    strncpy(reason_filled, reason_fmt.c_str(), STRING_LEN);
  }

  this->what_msg.reserve(func.size() + strlen(reason_filled) + 40);
  this->what_msg = "*(RUNTIME)* " + func + "(): " + reason_filled;
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
}

const char* libathome_common::Error::
what() const noexcept
{
  return this->what_msg.c_str();
}
