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


#include "libathome-common/RealtimeClock.hpp"
#include "libathome-common/Error.hpp"


const char* libathome_common::RealtimeClock::
to_string(RealtimeClock::timezone_t timezone)
{
  switch (timezone) {
  case utc_e: return "UTC";
  case local_e: return "local-time";
  }

  return "<not implemented!>";
}

libathome_common::RealtimeClock::
RealtimeClock() noexcept(false)
  :RealtimeClock(RealtimeClock::timezone_t::local_e)
{
}

libathome_common::RealtimeClock::
RealtimeClock(RealtimeClock::timezone_t timezone) noexcept(false)
{
  if (0 > ::time(&this->timestamp))
    throw Err("Could not fetch time from system RTC!");

  this->set_timezone(timezone);
}

libathome_common::RealtimeClock::
~RealtimeClock()
{
}

std::string libathome_common::RealtimeClock::
to_string(const std::string& strftime_fmt) const noexcept(false)
{
  string_t buf;

  if (0 >= ::strftime(
      buf, STRING_LEN, strftime_fmt.c_str(), &this->timestruct))
    throw Err("Could not convert time struct to string from format '%s'!",
              strftime_fmt.c_str());

  return std::string(buf);
}

void libathome_common::RealtimeClock::
set_timezone(RealtimeClock::timezone_t timezone) noexcept(false)
{
  this->timezone = timezone;

  /* Set this->timestruct
   */
#ifndef OSWIN /* Default case  */
  ::tm* timestruct_result = NULL;

  switch (this->timezone) {
  case timezone_t::utc_e:
    timestruct_result = ::gmtime_r(&timestamp, &this->timestruct);
    break;
  case timezone_t::local_e:
    timestruct_result = ::localtime_r(&timestamp, &this->timestruct);
    break;
  }

  bool iserror = timestruct_result != &this->timestruct;
#else /* Windows  */
  ::errno_t timestruct_result = 0;

  switch (this->timezone) {
  case timezone_t::utc_e:
    timestruct_result = ::gmtime_s(&this->timestruct, &timestamp);
    break;
  case timezone_t::local_e:
    timestruct_result = ::localtime_s(&this->timestruct, &timestamp);
    break;
  }

  bool iserror = timestruct_result != 0;
#endif /* #ifndef OSWIN  */

  if (iserror)
    throw Err("Could not convert unix timestamp to struct!");
}

libathome_common::RealtimeClock::timezone_t
libathome_common::RealtimeClock::
get_timezone() const
{
  return this->timezone;
}
