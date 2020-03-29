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


#include "RealtimeClock.hpp"


libathome_common::RealtimeClock::
RealtimeClock()
  :RealtimeClock(RealtimeClock::timezone_t::local_e)
{
}

libathome_common::RealtimeClock::
RealtimeClock(RealtimeClock::timezone_t timezone)
{
  if (0 > time(&this->timestamp)) {
    /* LOGGER may not be initialized.  So use FPRINTF for output here.
     */
    fprintf(stderr, "ERROR: Logger: Could not get time from RTC!\n");
    timestamp = (time_t) 0;
  }

  this->set_timezone(timezone);
}

libathome_common::RealtimeClock::
~RealtimeClock()
{
}

void libathome_common::RealtimeClock::
to_string(std::string& result, const std::string& fmt)
{
  string_t buf;

  if (0 == strftime(buf, STRING_LEN, fmt.c_str(), &this->timestruct)) {
    fprintf(stderr, "ERROR: Logger: Could not convert time struct to"
            " string!\n");
    return;
  }

  result = buf;
}

void libathome_common::RealtimeClock::
set_timezone(RealtimeClock::timezone_t timezone)
{
  this->timezone = timezone;

  /* Set this->timestruct
   */
#ifndef OSWIN /* Default case  */
  tm* timestruct_result = NULL;

  switch (this->timezone) {
  case timezone_t::utc_e:
    timestruct_result = gmtime_r(&timestamp, &this->timestruct);
    break;
  case timezone_t::local_e:
    timestruct_result = localtime_r(&timestamp, &this->timestruct);
    break;
  }

  bool iserror = timestruct_result != &this->timestruct;
#else /* Windows  */
  errno_t timestruct_result = 0;

  switch (this->timezone) {
  case timezone_t::utc_e:
    timestruct_result = gmtime_s(&this->timestruct, &timestamp);
    break;
  case timezone_t::local_e:
    timestruct_result = localtime_s(&this->timestruct, &timestamp);
    break;
  }

  bool iserror = timestruct_result != 0;
#endif /* #ifndef OSWIN  */

  if (iserror) {
    /* LOGGER may not be initialized.  So use FPRINTF for output here.
     */
    fprintf(stderr, "ERROR: Logger: Could not convert unix timestamp"
            " to struct!\n");
    return;
  }
}

libathome_common::RealtimeClock::timezone_t
libathome_common::RealtimeClock::
get_timezone()
{
  return this->timezone;
}
