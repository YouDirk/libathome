# lib@home, framework to develop distributed calculations.
# Copyright (C) 2020  Dirk "YouDirk" Lehmann
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.


LIBNAME = libathome-common
OBJ = Common Error RealtimeClock Filesystem File Logger

INCLUDE_PATHS = ..
LD_PATHS =
LIBS =

include ../../makeinc/makefile.inc.mk

# Compiling on Windows?
ifneq (,$(OS_IS_WIN))
LIBS += dbghelp
else
LIBS += dl
endif
