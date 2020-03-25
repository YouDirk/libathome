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
ERRB = $(NL)$(NL)  ERROR:


# ********************************************************************
# Linux/MSYS2 commands, feature check

ifneq (,$(_CACHE_FILE))

_CMD_TEST = $(shell which $(1) 2> /dev/null)

CC = $(call _CMD_TEST,g++)
ifeq (,$(CC))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) C++ compiler not found!  Try '$$> apt-get install \
    g++' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>')
else
  $(shell echo 'CC = $(CC)' >> $(_CACHE_FILE))
endif

AS = $(call _CMD_TEST,g++)
ifeq (,$(AS))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) ASM compiler not found!  Try '$$> apt-get install \
    g++' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>')
else
  $(shell echo 'AS = $(AS)' >> $(_CACHE_FILE))
endif

LD = $(call _CMD_TEST,g++)
ifeq (,$(LD))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) Linker command not found!  Try '$$> apt-get install \
    g++' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>')
else
  $(shell echo 'LD = $(LD)' >> $(_CACHE_FILE))
endif

SED = $(call _CMD_TEST,sed)
ifeq (,$(SED))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) SED command not found!  Try '$$> apt-get install \
    sed' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>')
else
  $(shell echo 'SED = $(SED)' >> $(_CACHE_FILE))
endif

TOUCH = $(call _CMD_TEST,touch)
ifeq (,$(TOUCH))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) TOUCH command not found!  Try '$$> apt-get install \
    coreutils' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>')
else
  $(shell echo 'TOUCH = $(TOUCH)' >> $(_CACHE_FILE))
endif

UNAME = $(call _CMD_TEST,uname)
ifeq (,$(UNAME))
  $(shell rm -f $(_CACHE_FILE))
  $(error $(ERRB) UNAME command not found!  Try '$$> apt-get install \
    coreutils' for installation.  Or use MSYS2 command \
    '$$> pacman -S <package>')
else
  $(shell echo 'UNAME = $(UNAME)' >> $(_CACHE_FILE))
endif

# --------------------------------------------------------------------

DEBUGGER_OPT = $(call _CMD_TEST,gdb)
ifeq (,$(DEBUGGER_OPT))
  $(shell echo 'DEBUGGER_OPT =' >> $(_CACHE_FILE))
else
  $(shell echo 'DEBUGGER_OPT = $(DEBUGGER_OPT)' >> $(_CACHE_FILE))
endif

VALGRIND_OPT = $(call _CMD_TEST,valgrind)
ifeq (,$(VALGRIND_OPT))
  $(shell echo 'VALGRIND_OPT =' >> $(_CACHE_FILE))
else
  $(shell echo 'VALGRIND_OPT = $(VALGRIND_OPT)' >> $(_CACHE_FILE))
endif

CTAGS_OPT = $(call _CMD_TEST,ctags)
ifeq (,$(CTAGS_OPT))
  $(shell echo 'CTAGS_OPT =' >> $(_CACHE_FILE))
else
  $(shell echo 'CTAGS_OPT = $(CTAGS_OPT)' >> $(_CACHE_FILE))
endif

ETAGS_OPT = $(call _CMD_TEST,etags)
ifeq (,$(ETAGS_OPT))
  $(shell echo 'ETAGS_OPT =' >> $(_CACHE_FILE))
else
  $(shell echo 'ETAGS_OPT = $(ETAGS_OPT)' >> $(_CACHE_FILE))
endif

EBROWSE_OPT = $(call _CMD_TEST,ebrowse)
ifeq (,$(EBROWSE_OPT))
  $(shell echo 'EBROWSE_OPT =' >> $(_CACHE_FILE))
else
  $(shell echo 'EBROWSE_OPT = $(EBROWSE_OPT)' >> $(_CACHE_FILE))
endif

DOXYGEN_OPT = $(call _CMD_TEST,doxygen)
ifeq (,$(DOXYGEN_OPT))
  $(shell echo 'DOXYGEN_OPT =' >> $(_CACHE_FILE))
else
  $(shell echo 'DOXYGEN_OPT = $(DOXYGEN_OPT)' >> $(_CACHE_FILE))
endif

# --------------------------------------------------------------------

OS_IS_WIN = $(shell test \( \
  -n "`echo $(OS) | $(SED) -n '/^windows/Ip'`" \
  -o -n "`$(UNAME) -o | $(SED) -n '/^msys/Ip'`" \
  -o -n "`$(UNAME) -o | $(SED) -n '/^mingw/Ip'`" \
  -o -n "`$(UNAME) -s | $(SED) -n '/^msys/Ip'`" \
  -o -n "`$(UNAME) -s | $(SED) -n '/^mingw/Ip'`" \
\) && echo -n 1)
$(shell echo 'OS_IS_WIN = $(OS_IS_WIN)' >> $(_CACHE_FILE))

endif # ifneq (,$(_CACHE_FILE))

# End of Linux/MSYS2 commands, feature check
# ********************************************************************

MAKEDEP := $(CC) $(CCFLAGS) -M

# ********************************************************************
