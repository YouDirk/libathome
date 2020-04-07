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


#ifndef LIBATHOME_COMMON_REALTIMECLOCK_H__
#define LIBATHOME_COMMON_REALTIMECLOCK_H__

#include "libathome-common/Common.hpp"

#include <ctime>  /* Same as <time.h>  */

namespace libathome_common
{

/**
 * Provides functionality of an RTC.
 *
 * The time provided is captured on instancing the class during
 * RealtimeClock::RealtimeClock().
 */
class RealtimeClock
{
public:

  /**
   * Represents the possible timezones.
   */
  typedef enum {
    utc_e = 0,  ///< Coordinated Universal Time
    local_e = 1 ///< Use local system settings (see environment var `TZ`)
  } timezone_t;

  /**
   * Convert a RealtimeClock::timezone_t to string.
   *
   * @param timezone The timezone to convert
   * @return The string which names the timezone. `static` allocated,
   *         need NOT to be `free()`d.
   */
  static const char* to_string(RealtimeClock::timezone_t timezone);

  /**
   * Fetch current time with timezone
   * RealtimeClock::timezone_t::local_e.
   *
   * But you can change the timezone later, using
   * RealtimeClock::set_timezone().
   *
   * @exception Error will be thrown if `timestamp` could not be
   *            fetched from system RTC
   */
  explicit RealtimeClock() noexcept(false);

  /**
   * Fetch current time with timezone `timezone`.
   *
   * @param timezone The timezone which should be used.
   * @exception Error will be thrown if `timestamp` could not be
   *            fetched from system RTC
   */
  explicit RealtimeClock(RealtimeClock::timezone_t timezone)
    noexcept(false);

  /**
   * Default destructor.
   */
  virtual ~RealtimeClock();

  /**
   * Output the fetched time.
   *
   * The `result` will be formated using `strftime_fmt`.  Take a look
   * to `$> man 3 strftime` for detailed information.  Make sure to
   * hold your format strings POSIX compatible and try to avoid other
   * extensions.
   *
   * @param strftime_fmt The format string.  Make sure to hold it
   *        POSIX compatible and try to avoid other extensions.
   * @return The resulting formatted string.
   * @exception Error will be thrown if some formatting of
   *            `strftime_fmt` is going wrong
   */
  virtual std::string
  to_string(const std::string& strftime_fmt) const noexcept(false);

  /**
   * Set `timezone`.
   *
   * @param timezone The timezone to set.
   * @exception Error will be thrown if `timestamp` could not be
   *            converted to localized `timestruct`
   */
  virtual void set_timezone(RealtimeClock::timezone_t timezone)
    noexcept(false);

  /**
   * Get `timezone`.
   *
   * @return The timezone to get.
   */
  virtual RealtimeClock::timezone_t get_timezone() const;

private:
  ::time_t timestamp;

  RealtimeClock::timezone_t timezone;
  ::tm timestruct;

}; /* class RealtimeClock  */

} /* namespace libathome_common  */
#endif /* LIBATHOME_COMMON_REALTIMECLOCK_H__  */
