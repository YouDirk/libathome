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


#ifndef LIBATHOME_CLIENT_INIT_H__
#define LIBATHOME_CLIENT_INIT_H__

#include <libathome-common.hpp>


/**
 * Represents the API of `libathome-client.so` or
 * `libathome-client.dll`.
 *
 * Here is all stuff implemented which is needed and used by the
 * client-side code.
 *
 * You need to `#include` it directly by `#include
 * <libathome-client.hpp>`.  `libathome-common.hpp` will be implicit
 * included, and depending on your system you need to tell the
 * compiler the correct **include path**, typically using the C++
 * compiler flag `-I/path/to/include`.  Additionally you need to tell
 * the linker to link to `libathome-client.{so,dll}`, typically using
 * the **linker flag** `-lathome-client` from **library path**
 * (optionally) `-L/path/to/lib`.
 *
 * Use the compiler flag `-DDEBUG` during development for things such
 * like to force `libathome_common::Log` output to `/dev/stdout`,
 * enable logging backtrace at runtime error, etc ...
 *
 * Use the linker flag `-rdynamic` optionally on Linux or Unix-like
 * systems to make backtraces of libathome_common::Error more readable
 * for humans.
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
 * $> g++ -rdynamic -L/path/to/lib -o myproject-client main.o \
 *    Class1.o ... -lathome-common -lathome-client
 * ```
 */
namespace libathome_client
{

/**
 * Initialisator class, needed for everything :P
 *
 * Also implicit includes STL stuff, stdlibs, etc ...
 */
class Init: public libathome_common::Common
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
  explicit Init(int argc, char** argv);
  /**
   * Bye bye, free memory and let forget all.
   */
  virtual ~Init();

}; /* class Init  */

} /* namespace libathome_client  */
#endif /* LIBATHOME_CLIENT_INIT_H__  */
