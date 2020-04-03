# lib@home, framework to develop distributed calculations.
# Copyright (C) 2019  Dirk "YouDirk" Lehmann
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


_BLANK :=
define NL

$(_BLANK)
endef
ERRB  = $(NL)$(NL)  ERROR:
WARNB = $(NL)$(NL)  warning:

# ********************************************************************
# Linux/MSYS2 commands, feature check

ifneq (,$(_CACHE_FILE))

_CMD_TEST = \
  $(eval _CMD_TEST_BUF:=checking for $(2)... )$(\
  )$(shell which $(1) 2> /dev/null)
_CMD_TEST_NOOUT = $(shell which $(1) 2> /dev/null)
_CMD_TEST_RESULT = $(info $(_CMD_TEST_BUF)$(1))
_CMD_TEST_RESNO = $(info $(_CMD_TEST_BUF)no - Debian package $(1))

CC := $(call _CMD_TEST,g++,C++ compiler)
ifeq (,$(CC))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) C++ compiler not found!  Try '$$> apt-get install \
    g++' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>'.  After install run '$$> make clean-all')
else
  $(call _CMD_TEST_RESULT,$(CC))
  $(shell echo 'CC = $(CC)' >> $(_CACHE_FILE))
endif

AS := $(call _CMD_TEST,g++,ASM compiler)
ifeq (,$(AS))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) ASM compiler not found!  Try '$$> apt-get install \
    g++' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>'.  After install run '$$> make clean-all')
else
  $(call _CMD_TEST_RESULT,$(AS))
  $(shell echo 'AS = $(AS)' >> $(_CACHE_FILE))
endif

LD := $(call _CMD_TEST,g++,linker)
ifeq (,$(LD))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) Linker command not found!  Try '$$> apt-get install \
    g++' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>'.  After install run '$$> make clean-all')
else
  $(call _CMD_TEST_RESULT,$(LD))
  $(shell echo 'LD = $(LD)' >> $(_CACHE_FILE))
endif

SED := $(call _CMD_TEST,sed,sed)
ifeq (,$(SED))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) SED command not found!  Try '$$> apt-get install \
    sed' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>'.  After install run '$$> make clean-all')
else
  $(call _CMD_TEST_RESULT,$(SED))
  $(shell echo 'SED = $(SED)' >> $(_CACHE_FILE))
endif

TOUCH := $(call _CMD_TEST,touch,touch)
ifeq (,$(TOUCH))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) TOUCH command not found!  Try '$$> apt-get install \
    coreutils' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>'.  After install run '$$> make clean-all')
else
  $(call _CMD_TEST_RESULT,$(TOUCH))
  $(shell echo 'TOUCH = $(TOUCH)' >> $(_CACHE_FILE))
endif

UNAME := $(call _CMD_TEST,uname,uname)
ifeq (,$(UNAME))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) UNAME command not found!  Try '$$> apt-get install \
    coreutils' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>'.  After install run '$$> make clean-all')
else
  $(call _CMD_TEST_RESULT,$(UNAME))
  $(shell echo 'UNAME = $(UNAME)' >> $(_CACHE_FILE))
endif

# --------------------------------------------------------------------

DEBUGGER_OPT := $(call _CMD_TEST,gdb,debugger (optional))
ifeq (,$(DEBUGGER_OPT))
  $(call _CMD_TEST_RESNO,gdb)
  $(shell echo 'DEBUGGER_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(DEBUGGER_OPT))
  $(shell echo 'DEBUGGER_OPT = $(DEBUGGER_OPT)' >> $(_CACHE_FILE))
endif

VALGRIND_OPT := $(call _CMD_TEST,valgrind,Valgrind (optional))
ifeq (,$(VALGRIND_OPT))
  $(call _CMD_TEST_RESNO,valgrind)
  $(shell echo 'VALGRIND_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(VALGRIND_OPT))
  $(shell echo 'VALGRIND_OPT = $(VALGRIND_OPT)' >> $(_CACHE_FILE))
endif

CTAGS_OPT := $(call _CMD_TEST,ctags,ctags (optional))
ifeq (,$(CTAGS_OPT))
  $(call _CMD_TEST_RESNO,emacs-bin-common)
  $(shell echo 'CTAGS_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(CTAGS_OPT))
  $(shell echo 'CTAGS_OPT = $(CTAGS_OPT)' >> $(_CACHE_FILE))
endif

ETAGS_OPT := $(call _CMD_TEST,etags,etags (optional))
ifeq (,$(ETAGS_OPT))
  $(call _CMD_TEST_RESNO,emacs-bin-common)
  $(shell echo 'ETAGS_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(ETAGS_OPT))
  $(shell echo 'ETAGS_OPT = $(ETAGS_OPT)' >> $(_CACHE_FILE))
endif

EBROWSE_OPT := $(call _CMD_TEST,ebrowse,ebrowse (optional))
ifeq (,$(EBROWSE_OPT))
  $(call _CMD_TEST_RESNO,emacs-bin-common)
  $(shell echo 'EBROWSE_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(EBROWSE_OPT))
  $(shell echo 'EBROWSE_OPT = $(EBROWSE_OPT)' >> $(_CACHE_FILE))
endif

DOXYGEN_OPT := $(call _CMD_TEST,doxygen,Doxygen (optional))
ifeq (,$(DOXYGEN_OPT))
  $(call _CMD_TEST_RESNO,doxygen)
  $(shell echo 'DOXYGEN_OPT =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,$(DOXYGEN_OPT))
  $(shell echo 'DOXYGEN_OPT = $(DOXYGEN_OPT)' >> $(_CACHE_FILE))
endif

# --------------------------------------------------------------------

BROWSER_OPT := \
  "$(call _CMD_TEST,/usr/bin/firefox,web browser (optional))"
ifeq ("",$(BROWSER_OPT))
  BROWSER_OPT := \
    "$(call _CMD_TEST_NOOUT,/c/Program\ Files/Mozilla\ Firefox$(\
      )/firefox.exe)"
endif
ifeq ("",$(BROWSER_OPT))
  BROWSER_OPT := \
    "$(call _CMD_TEST_NOOUT,/c/Program\ Files\ \(x86\)/$(\
      )Mozilla\ Firefox/firefox.exe)"
endif
ifeq ("",$(BROWSER_OPT))
  BROWSER_OPT := \
    "$(call _CMD_TEST_NOOUT,/c/Program\ Files/Internet\ Explorer$(\
      )/iexplore.exe)"
endif
ifeq ("",$(BROWSER_OPT))
  BROWSER_OPT := \
    "$(call _CMD_TEST_NOOUT,/c/Program\ Files\ \(x86\)/$(\
      )Internet\ Explorer/iexplore.exe)"
endif
ifeq ("",$(BROWSER_OPT))
  $(call _CMD_TEST_RESNO,firefox-esr)
  BROWSER_OPT =
else
  $(call _CMD_TEST_RESULT,$(BROWSER_OPT))
endif
$(shell echo 'BROWSER_OPT = $(BROWSER_OPT) ' >> $(_CACHE_FILE))

# --------------------------------------------------------------------

OS_IS_WIN := := $(call _CMD_TEST,loooool,operating system)
OS_IS_WIN := $(shell test \( \
  -n "`echo $(OS) | $(SED) -n '/^windows/Ip'`" \
  -o -n "`$(UNAME) -o | $(SED) -n '/^msys/Ip'`" \
  -o -n "`$(UNAME) -o | $(SED) -n '/^mingw/Ip'`" \
  -o -n "`$(UNAME) -s | $(SED) -n '/^msys/Ip'`" \
  -o -n "`$(UNAME) -s | $(SED) -n '/^mingw/Ip'`" \
\) && echo -n 1)
ifeq (,$(OS_IS_WIN))
  $(call _CMD_TEST_RESULT,Linux/Unix-like)
  $(shell echo 'OS_IS_WIN =' >> $(_CACHE_FILE))
else
  $(call _CMD_TEST_RESULT,MSYS2/MinGW/Windows)
  $(shell echo 'OS_IS_WIN = $(OS_IS_WIN)' >> $(_CACHE_FILE))
endif

endif # ifneq (,$(_CACHE_FILE))

# End of Linux/MSYS2 commands, feature check
# ********************************************************************

MAKEDEP := $(CC) $(CCFLAGS) -M

# ********************************************************************
