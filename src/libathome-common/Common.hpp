/* `lib@home`, a distributed calculations framework
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


#ifndef LIBATHOME_COMMON_COMMON_H__
#define LIBATHOME_COMMON_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace libathome_common
{

class Common
{
public:
  static const unsigned            STRING_LEN = 256;

  explicit Common(int argc, char** argv);
  virtual ~Common();

private:
  char* hello;

}; /* class Common  */

typedef char                       string_t[Common::STRING_LEN];

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_COMMON_H__  */