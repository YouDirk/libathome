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


#include "libathome-common/Error.hpp"

#if defined __GNUC__ && !defined OSWIN
#  /* For backtrace stuff, GNU extension && Linux  */
#  include <execinfo.h>
#  include <dlfcn.h>
#  include <cxxabi.h>
#elif defined OSWIN
#  /* For backtrace stuff, Windows using StackWalk64 API  */
#  include <windows.h>
#  include <dbghelp.h>
#endif /* ifdef __GNUC__  */


const std::regex libathome_common::Error::
REGEX_FUNCNAME("^(.* )?([^ (]*)\\(.*$");

const std::regex libathome_common::Error::
REGEX_LIBNAME("^(.*[/\\\\])?([^/\\\\].*)$");

/* ---------------------------------------------------------------  */

void libathome_common::Error::
_init(bool _backtrace_append,
  const char* _pretty_func, const char* reason_fmt, va_list ap)
{
  this->backtrace_size
    = this->_backtrace(this->backtrace_frames, Error::BACKTRACE_MAX);
  this->backtrace_symbolz = this->
    _backtrace_symbols(this->backtrace_frames, this->backtrace_size);

  /* ---  */

  std::string func = _pretty_func != NULL
    ? std::regex_replace(_pretty_func, Error::REGEX_FUNCNAME, "$2")
    : "";

  if (func.empty())
    func = _pretty_func != NULL? _pretty_func: "???";

  string_t buf;
  if (0 >= ::vsnprintf(buf, STRING_LEN, reason_fmt, ap)) {
    /* Throwing Error in Error is a bad idea.  So we are making the
     * best what is possible.
     */
    ::strncpy(buf, reason_fmt, STRING_LEN-1);
  }

  this->what_msg = "*(RUNTIME)* " + func + "(): " + buf;

  /* ---  */

  this->backtrace_appended = false;
  if (_backtrace_append) this->bt();
} /* _init()  */

int libathome_common::Error::
_backtrace(void** buffer, int size)
{
#if defined __GNUC__ && !defined OSWIN
  return ::backtrace(buffer, size);
#elif defined OSWIN
  /* The StackWalk64 API reference you can find here:
   *
   * https://docs.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-stackwalk64
   */
  ::HANDLE process = ::GetCurrentProcess();
  if (!::SymInitialize(process, NULL, true)) {
    buffer[0] = NULL;
    return 0;
  }
  ::HANDLE thread = ::GetCurrentThread();

  ::CONTEXT context;
  ::memset(&context, 0, sizeof(::CONTEXT));
  context.ContextFlags = CONTEXT_FULL;
  ::RtlCaptureContext(&context);

  ::DWORD image;

  ::STACKFRAME64 stackframe;
  ::memset(&stackframe, 0, sizeof(::STACKFRAME64));
#ifdef _M_IX86
  image = IMAGE_FILE_MACHINE_I386;
  stackframe.AddrPC.Offset = context.Eip;
  stackframe.AddrPC.Mode = AddrModeFlat;
  stackframe.AddrFrame.Offset = context.Ebp;
  stackframe.AddrFrame.Mode = AddrModeFlat;
  stackframe.AddrStack.Offset = context.Esp;
  stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
  image = IMAGE_FILE_MACHINE_AMD64;
  stackframe.AddrPC.Offset = context.Rip;
  stackframe.AddrPC.Mode = AddrModeFlat;
  stackframe.AddrFrame.Offset = context.Rsp;
  stackframe.AddrFrame.Mode = AddrModeFlat;
  stackframe.AddrStack.Offset = context.Rsp;
  stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
  image = IMAGE_FILE_MACHINE_IA64;
  stackframe.AddrPC.Offset = context.StIIP;
  stackframe.AddrPC.Mode = AddrModeFlat;
  stackframe.AddrFrame.Offset = context.IntSp;
  stackframe.AddrFrame.Mode = AddrModeFlat;
  stackframe.AddrBStore.Offset = context.RsBSP;
  stackframe.AddrBStore.Mode = AddrModeFlat;
  stackframe.AddrStack.Offset = context.IntSp;
  stackframe.AddrStack.Mode = AddrModeFlat;
#else /* machine type  */
#warning "Backtraces not supported!  Supported are x86, amd64, ia64."
  buffer[0] = NULL;
  return 0;
#endif /* machine type  */

  /* Iterate through `stackframe` via `StackWalk64()`  */
  int i=0;
  for (; i<size; i++) {
    if (!::StackWalk64(image, process, thread, &stackframe, &context,
         NULL, ::SymFunctionTableAccess64, ::SymGetModuleBase64, NULL))
      break;

    buffer[i] = (void*) stackframe.AddrPC.Offset;
  }

  ::SymCleanup(process);
  return i;
#else /* elif defined OSWIN  */
#warning "Backtraces are not implemented for your compiler or OS!"
  buffer[0] = NULL;
  return 0;
#endif /* ifdef __GNUC__  */
} /* _backtrace()  */

char** libathome_common::Error::
_backtrace_symbols(void* const* buffer, int size)
{
  if (size <= 0) return NULL;

  typedef struct {
    char* ptr[Error::BACKTRACE_MAX];
    string_t strings[Error::BACKTRACE_MAX];
  } symbols_t;


#if defined __GNUC__ && !defined OSWIN
  char** symbols_fallback = ::backtrace_symbols(buffer, size);
  symbols_t* result = (symbols_t*) ::malloc(sizeof(symbols_t));

  ::Dl_info dlinfo;
  char* demangle_cur = NULL;
  size_t demangle_length = 0;
  int demangle_status;
  for (int i=0; i<size; i++) {
    result->ptr[i] = result->strings[i];

    if (!::dladdr(buffer[i], &dlinfo) || dlinfo.dli_sname == NULL) {
      ::snprintf(
        result->strings[i], STRING_LEN, "%s", symbols_fallback[i]);

      continue;
    }

    std::string libname =
      std::regex_replace(dlinfo.dli_fname, Error::REGEX_LIBNAME, "$2");
    unsigned long offset
      = (unsigned long) buffer[i] - (unsigned long) dlinfo.dli_saddr;

    char* dem_result = abi::__cxa_demangle(dlinfo.dli_sname,
      demangle_cur, &demangle_length, &demangle_status);
    if (dem_result == NULL) {
      ::snprintf(result->strings[i], STRING_LEN, "%20s::%s()+0x%02lx [%p]",
        libname.c_str(), dlinfo.dli_sname, offset, buffer[i]);

      continue;
    }
    demangle_cur = dem_result;

    ::snprintf(result->strings[i], STRING_LEN, "%20s::%s+0x%02lx [%p]",
      libname.c_str(), demangle_cur, offset, buffer[i]);
  } /* for (int i=0; i<size; i++)  */

  ::free(demangle_cur);
  ::free(symbols_fallback);
  return result->ptr;
#elif defined OSWIN
  /* The StackWalk64 API reference you can find here:
   *
   * https://docs.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-stackwalk64
   */
  ::HANDLE process = ::GetCurrentProcess();
  if (!::SymInitialize(process, NULL, true)) return NULL;

  ::IMAGEHLP_MODULE64 module;
  module.SizeOfStruct = sizeof(::IMAGEHLP_MODULE64);

  char symbol_buf[
    sizeof(::IMAGEHLP_SYMBOL64) + MAX_SYM_NAME*sizeof(::TCHAR)];
  ::PIMAGEHLP_SYMBOL64 symbol = (::IMAGEHLP_SYMBOL64*) symbol_buf;
  symbol->SizeOfStruct = sizeof(::IMAGEHLP_SYMBOL64);
  symbol->MaxNameLength = MAX_SYM_NAME;

  ::IMAGEHLP_LINE64 fileline;
  fileline.SizeOfStruct = sizeof(::IMAGEHLP_LINE64);

  symbols_t* result = (symbols_t*) ::malloc(sizeof(symbols_t));

  for (int i=0; i<size; i++) {
    result->ptr[i] = result->strings[i];
    ::DWORD64 cur_stackframe = (::DWORD64) buffer[i];

    if (!::SymGetModuleInfo64(process, cur_stackframe, &module))
      ::strncpy(module.ModuleName, "<unknown module>", 32-1);

    ::DWORD64 displacement = 0;
    if (!::SymGetSymFromAddr64(
           process, cur_stackframe, &displacement, symbol)) {
      ::snprintf(result->strings[i], STRING_LEN,
        "%s [0x%01x%07x]", module.ModuleName,
        (uint32_t) (cur_stackframe >> 32), (uint32_t) cur_stackframe);

      continue;
    }

    if (!::SymGetLineFromAddr64(
         process, cur_stackframe, (::PDWORD) &displacement, &fileline)) {
      ::snprintf(result->strings[i], STRING_LEN,
        "%s::%s()+0x%02x [0x%01x%07x]", module.ModuleName, symbol->Name,
        (uint32_t) displacement, (uint32_t) (cur_stackframe >> 32),
        (uint32_t) cur_stackframe);

      continue;
    }

    ::snprintf(result->strings[i], STRING_LEN,
      "%20s:%03lu: %s::%s() [0x%01x%07x]", fileline.FileName,
      fileline.LineNumber, module.ModuleName, symbol->Name,
      (uint32_t) (cur_stackframe >> 32), (uint32_t) cur_stackframe);
  } /* for (int i=0; i<size; i++)  */

  ::SymCleanup(process);
  return result->ptr;
#else /* elif defined OSWIN  */
  return NULL;
#endif /* ifdef __GNUC__  */
} /* _backtrace_symbols()  */

/* ---------------------------------------------------------------  */

libathome_common::Error::
Error(bool _backtrace_append, const char* _pretty_func,
      const std::string& reason)
  :Error(_backtrace_append, _pretty_func, "%s", reason.c_str())
{
}

libathome_common::Error::
Error(bool _backtrace_append, const char* _pretty_func,
      const char* reason_fmt, ...)
  :std::runtime_error(std::string(reason_fmt))
{
  ::va_list ap;

  ::va_start(ap, reason_fmt);
  this->_init(_backtrace_append, _pretty_func, reason_fmt, ap);
  ::va_end(ap);
}

libathome_common::Error::
~Error()
{
  if (this->backtrace_symbolz) ::free(this->backtrace_symbolz);
}

/* ---------------------------------------------------------------  */

void libathome_common::Error::
bt() noexcept
{
  /* Only one time plz  */
  if (this->backtrace_appended) return;
  this->backtrace_appended = true;

  this->what_msg += "\n\nbacktrace:";

  if (this->backtrace_size == 0)
    this->what_msg += "\n  <Not implemented for your compiler or OS>";

  string_t buf;
  int output_size = this->get_backtrace_size();
  for (int i=0; i<output_size; i++) {
    if (0 >= ::snprintf(buf, STRING_LEN, "\n  %s",
               this->backtrace_symbolz[i + Error::BACKTRACE_OFFSET])) {
      /* Throwing Error in Error is a bad idea.  So we are making the
       * best what is possible.
       */
      continue;
    }
    this->what_msg += buf;
  }

  this->what_msg += this->is_backtrace_more()
    ? "\n      (...)\n": "\n";
}

const char* libathome_common::Error::
what() const noexcept
{
  return this->what_msg.c_str();
}

int libathome_common::Error::
get_backtrace_size() const noexcept
{
  int result = this->backtrace_size - Error::BACKTRACE_OFFSET;

  return result < 0? 0: result;
}
bool libathome_common::Error::
is_backtrace_more() const noexcept
{
  return this->backtrace_size == Error::BACKTRACE_MAX;
}
const void* libathome_common::Error::
get_backtrace_frame(int i) const noexcept
{
  /* Don't THROW, because an Error in Error is a bad idea o.o  */
  if (i < 0 || i >= (this->backtrace_size - Error::BACKTRACE_OFFSET))
    return NULL;

  return this->backtrace_frames[i + Error::BACKTRACE_OFFSET];
}
const char* libathome_common::Error::
get_backtrace_symbol(int i) const noexcept
{
  /* Don't THROW, because an Error in Error is a bad idea o.o  */
  if (i < 0 || i >= (this->backtrace_size - Error::BACKTRACE_OFFSET))
    return NULL;

  return this->backtrace_symbolz[i + Error::BACKTRACE_OFFSET];
}
