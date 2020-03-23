# lib@home, framework to develop distributed calculations.
# Copyright (C) 2020  Dirk "YouDirk" Lehmann
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


# ********************************************************************
# Variable definitions of all static stuff

# Debug build?
ifeq (1,$(DEBUG_BUILD))
  DEBUGFLAGS := -g
  DEBUGMACROS := -DDEBUG
else
  OPTFLAG := -Ofast
  DEBUGFLAGS := -g  # Provide debugging symbols in productive builds
  DEBUGMACROS :=
endif

# Compiling library?
ifneq (,$(LIBNAME))
  OUTPUT := $(LIBNAME).so
  MAIN_HEADER := $(LIBNAME).hpp
  FPICFLAGS := -fPIC -DPIC
  SHAREDFLAGS := -shared
else
  OUTPUT := $(PROJECT_DIRNAME)-client
  MAIN_HEADER :=
  FPICFLAGS :=
  SHAREDFLAGS :=
endif

EBROWSEFILE := BROWSE
CTAGSFILE := tags
ETAGSFILE := TAGS
MAKEFILEZ := $(addprefix ../../makeinc/, \
  ../makefile.config.mk makefile.inc.mk makefile.inc-global.mk \
  makefile.check.mk makefile.variables.mk)

CEXT := cpp
HEXT := hpp
SEXT := S
OEXT := o
DEPEXT := d
LOGEXT := log

OBJFILES := $(OBJ:=.$(OEXT))
DEPFILES := $(OBJ:=.$(DEPEXT))
HFILES := $(OBJ:=.$(HEXT))

FLAGS := $(DEBUGFLAGS) $(WARNFLAGS) $(OPTFLAG)
CCFLAGS := $(FLAGS) -c $(FPICFLAGS) $(DEBUGMACROS) \
           $(addprefix -I,$(INCLUDE_PATHS))
ASFLAGS := $(CCFLAGS)
LDFLAGS := $(FLAGS) $(SHAREDFLAGS) $(addprefix -L,$(LD_PATHS))
DEBUGGERFLAGS := --quiet -x ../../makeinc/batch.gdbinit

NULLCHAR :=
RUN_ENV := \
  LD_LIBRARY_PATH=$(subst $(NULLCHAR) ,:,$(LD_PATHS)):$$LD_LIBRARY_PATH

TAGEDFILES := $(wildcard *.$(CEXT) *.$(HEXT) *.$(SEXT))

CTAGSFLAGS :=
ETAGSFLAGS :=
EBROWSEFLAGS :=

# End of Variable definitions of all static stuff
# ********************************************************************
