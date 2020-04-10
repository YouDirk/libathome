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


#include "libathome-common/Common.hpp"
#include "libathome-common/Logger.hpp"


libathome_common::Common::
Common(int argc, char** argv)
{
#ifndef DEBUG
  libathome_common::Log = new Logger(
    Logger::loglevel_t::info_e, RealtimeClock::timezone_t::local_e,
    "log", "%Y-%m-%d.log", 365);
#else /* ifndef DEBUG  */
  libathome_common::Log = new Logger(
    Logger::loglevel_t::all_e, RealtimeClock::timezone_t::local_e);
#endif /* ifndef DEBUG  */

  this->hello = new std::string("Hello World!");
  /* Prevent unsued warning for now  */
  if (argc || argv) {};

  try {
    File x(NULL, "<nullstream>");
  } catch (Error& e) {
    Log->error(e);
  }

  Log->debug("%s", this->hello->c_str());
  Log->warn(*this->hello);
  Log->error("Hello %s, how are you (%d)?", "World", -999);
  //Log->fatal(3, this->hello->c_str());
}

libathome_common::Common::
~Common()
{
  delete this->hello;

  delete libathome_common::Log;
}
