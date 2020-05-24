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
/**
 * @file
 * @brief Documentation of libathome_client and declares the class
 *        libathome_client::Init.
 *
 * @dir
 * @brief Holds all files for namespace libathome_client.
 *
 * Include libathome-client.hpp (*implicitly includes
 * libathome-common.hpp*) to use all header files in this directory
 * and thereby all the classes using the namespace libathome_client
 * which represents the API of `libathome-client.so` or
 * `libathome-client.dll`.
 *
 * ```cpp
 * // Incudes all files from directory 'libathome-client/' and implicitly
 * // 'libathome-common/'
 *
 * #include <libathome-client.hpp>
 * ```
 */

#include <libathome-common.hpp>


/**
 * Represents the API of `libathome-client.so` or
 * `libathome-client.dll`.
 *
 * Here is all stuff implemented which is needed and used by the
 * client-side code.
 *
 * You need to `#include` it directly by `#include
 * <libathome-client.hpp>`.  `libathome-common.hpp` will be implicitly
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
 * Singleton initialisator class, needed for everything :P
 *
 * Just one instance per process must exitst.  Instantiation should be
 * one of first things in your `int main(int argc, char** argv)`
 * function.  Use Common::get() (or Init::get()) static method to get
 * the singleton instance from anywhere in your program.  Also
 * implicitly includes STL stuff, stdlibs, etc.
 */
class Init: public libathome_common::Common
{
public:

  /**
   * Singleton getter.
   *
   * @return The only one instance of this singleton, otherwise `NULL`
   */
  static Init* get();

  /**
   * Initialisator which test for API Version compatibility,
   * initializes all singletons, etc.
   *
   * @param argc Pass the first parameter of `int main(int argc, char**
   *             argv)` here
   * @param argv Pass the second parameter of `int main(int argc, char**
   *             argv)` here
   * @exception Error will be thrown if this process does already
   *            instanced a class of type libathome_common::Common
   */
  explicit Init(int argc, char** argv) noexcept(false);
  /**
   * Bye bye, free memory and let forget all.
   */
  virtual ~Init();

private:
  virtual void _abstract_class() override;
}; /* class Init  */

} /* namespace libathome_client  */
#endif /* LIBATHOME_CLIENT_INIT_H__  */
