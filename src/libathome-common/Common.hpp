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


#ifndef LIBATHOME_COMMON_COMMON_H__
#define LIBATHOME_COMMON_COMMON_H__

#include <cstdio>   /* Same as <stdio.h>  */
#include <cstdarg>  /* Same as <stdarg.h>  */
#include <cstring>  /* Same as <string.h>  */

#include <string>
#include <stdexcept>

/* Ignore __ATTRIBUTE__, if not using g++  */
#ifndef __GNUC__
#  define  __attribute__(x)
#endif


/**
 * Represents the API of `libathome-common.so` or
 * `libathome-common.dll`.
 *
 * Here is all stuff implemented which is shared, needed and used by
 * both: server-side code and client-side code.
 *
 * You don't need to `#include` it directly, because it will be
 * implicit included by `#include <libathome-client.hpp>` and
 * `#include <libathome-server.hpp>`.  But depending on your system
 * you need to tell the compiler the correct **include path**,
 * typically using the C++ compiler flag `-I/path/to/include`.
 * Additionally you need to tell the linker to link to
 * `libathome-common.{so,dll}`, typically using the **linker flag**
 * `-lathome-common` from **library path** (optionally)
 * `-L/path/to/lib`.
 *
 * Use the compiler flag `-DDEBUG` during development for things such
 * like force `libathome_common::Log` to output to `/dev/stdout`, etc
 * ...
 *
 * **Example `main.cpp` C++ file**
 * ```cpp
 * #include <libathome-client.hpp>
 *
 * using namespace libathome_common;
 * using namespace libathome_client;
 *
 * int
 * main(int argc, char** argv)
 * {
 *   Init* init = new Init(argc, argv, ...);
 *
 *   delete init;
 *   return 0;
 * }
 * ```
 *
 * **Example `g++` compile**
 * ```shell
 * $> g++ -c -DDEBUG -I/path/to/include -o main.o main.cpp
 * ```
 *
 * **Example `g++` linking**
 * ```shell
 * $> g++ -L/path/to/lib -o myproject-client main.o Class1.o ... \
 *    -lathome-common -lathome-client
 * ```
 */
namespace libathome_common
{

/**
 * Buffer size of static string type libathome_common::string_t.
 */
const unsigned STRING_LEN          = 256;

/**
 * Simple static string type.
 *
 * Is very compatible with ANSI C string funcions but has no features
 * and allocates much memory on stack!
 */
typedef char                       string_t[STRING_LEN];

/**
 * Initialisator class, needed for everything :P
 *
 * Also implicit includes STL stuff, stdlibs, etc ...
 */
class Common
{
public:
  /**
   * Initialisator which test for API Version compatibility,
   * initializes all singletons, etc.
   *
   * @param argc Pass the first parameter of `int main(int argc, char**
   *             argv)` here
   *
   * @param argv Pass the second parameter of `int main(int argc, char**
   *             argv)` here
   */
  explicit Common(int argc, char** argv);
  /**
   * Bye bye, free memory and let forget all.
   */
  virtual ~Common();

private:
  std::string* hello;

}; /* class Common  */

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_COMMON_H__  */
