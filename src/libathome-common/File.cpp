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


#include "libathome-common/File.hpp"
#include "libathome-common/Error.hpp"
#include "libathome-common/Filesystem.hpp"

#include <cerrno>


libathome_common::File::
File(::FILE* fstream, const std::string& stream_name) noexcept(false)
  :extern_fstream(fstream), binary(true), path("<no path>"),
   filename("<extern fstream>"), fstream(NULL),
   mode(File::access_t::read_e)
{
  if (fstream == NULL) {
    throw
      Err("Argument FSTREAM '%s' was set to NULL!", stream_name.c_str());
  }

  this->filename_full = stream_name;
}

libathome_common::File::
File(const std::string& path, const std::string& filename, bool binary)
  :extern_fstream(NULL), binary(binary), path(path), filename(filename),
   fstream(NULL), mode(File::access_t::read_e)
{
  this->filename_full
    = this->path + Filesystem::PATH_SEPERATOR + this->filename;
}

libathome_common::File::
~File()
{
  this->close();
}

/* ***************************************************************  */

const char* libathome_common::File::
to_string(File::access_t access)
{
  switch (access) {
  case read_e: return "read";
  case write_e: return "write";
  case append_e: return "append";
  }

  return "<not implemented!>";
}

void libathome_common::File::
open(File::access_t mode) noexcept(false)
{
  this->mode = mode;

  if (this->extern_fstream != NULL) {
    this->fstream = this->extern_fstream;
    return;
  }

  /* Don't Log here, only throw exceptions.  It's part of writing log!
   */
  if (this->mode == File::access_t::write_e ||
      this->mode == File::access_t::append_e) {
    Filesystem::mkdir(this->path);
  }

  char mode_str[3] = {'r', '\0', '\0'};
  if (this->binary) mode_str[1] = 'b';

  switch (mode) {
  case read_e:   mode_str[0] = 'r'; break;
  case write_e:  mode_str[0] = 'w'; break;
  case append_e: mode_str[0] = 'a'; break;
  }

  this->fstream = fopen(this->filename_full.c_str(), mode_str);
  if (this->fstream == NULL) {
    throw Err("Could not open file '%s' for '%s': %s!",
              this->filename_full.c_str(), File::to_string(this->mode),
              ::strerror(errno));
  }
}

void libathome_common::File::
close()
{
  /* Ignore double closes  */
  if (this->fstream == NULL) return;

  if (this->extern_fstream == NULL) fclose(this->fstream);

  this->mode = File::access_t::read_e;
  this->fstream = NULL;
}

/* ***************************************************************  */

void libathome_common::File::
vprintf(const char* fmt, ::va_list ap) const noexcept(false)
{
  if (this->fstream == NULL
      || (this->mode != File::access_t::write_e &&
          this->mode != File::access_t::append_e)) {
    throw Err("File '%s' not opened for write- or append-access!",
              this->filename_full.c_str());
  }

  if (0 >= ::vfprintf(this->fstream, fmt, ap))
    throw Err("Could not write to '%s'!", this->filename_full.c_str());
}

/* ***************************************************************  */

void libathome_common::File::
printf(const char* fmt, ...) const noexcept(false)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(fmt, ap);
  ::va_end(ap);
}

void libathome_common::File::
printf(const std::string& fmt, ...) const noexcept(false)
{
  ::va_list ap;

  ::va_start(ap, fmt);
  this->vprintf(fmt.c_str(), ap);
  ::va_end(ap);
}
