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

libathome_common::Error::
Error(const char* _pretty_func, const std::string& reason)
  :std::runtime_error(reason)
{
  std::string func = _pretty_func != NULL
    ? std::regex_replace(_pretty_func, Error::REGEX_FUNCNAME, "$2")
    : "";

  if (func.empty())
    func = _pretty_func != NULL? _pretty_func: "???";

  this->what_msg = "*(RUNTIME)* " + func + "(): " + reason;
}

libathome_common::Error::
Error(const char* _pretty_func, const char* reason)
  :Error(_pretty_func, std::string(reason))
{
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
