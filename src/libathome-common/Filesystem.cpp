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


#include "libathome-common/Filesystem.hpp"
#include "libathome-common/Error.hpp"

#include <sys/stat.h>
#include <cerrno>


#ifndef OSWIN
const char* libathome_common::Filesystem::PATH_SEPERATOR = "/";
#else /* ifndef OSWIN  */
const char* libathome_common::Filesystem::PATH_SEPERATOR = "\\";
#endif /* ifndef OSWIN  */

const char* libathome_common::Filesystem::PATH_DOT = ".";
const char* libathome_common::Filesystem::PATH_DOTDOT = "..";
const unsigned libathome_common::Filesystem::_UMODE_DEFAULT = 0777;


bool libathome_common::Filesystem::
mkdir(const std::string& path) noexcept(false)
{
#ifndef OSWIN
  int mkdir_res = ::mkdir(path.c_str(), Filesystem::_UMODE_DEFAULT);
#else /* ifndef OSWIN  */
  int mkdir_res = ::mkdir(path.c_str());
#endif /* ifndef OSWIN  */

  if (0 != mkdir_res) {
    if (errno == EEXIST) return false;

    throw Err("Could not create directory '%s': %s!", path.c_str(),
              ::strerror(errno));
  }

  return true;
}
