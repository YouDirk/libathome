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

ifeq (,$(OS_IS_WIN))
  SOEXT = so
  DOT_BINEXT =
else
  SOEXT = dll
  DOT_BINEXT = .exe
endif

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
  OUTPUT := $(LIBNAME).$(SOEXT)
  MAIN_HEADER := $(LIBNAME).hpp
  FPICFLAGS := -fPIC -DPIC
  SHAREDFLAGS := -shared
  DOC_OUTDIR := libathome
else
  OUTPUT := $(PROJECT_EXECNAME)-client$(DOT_BINEXT)
  MAIN_HEADER :=
  FPICFLAGS :=
  SHAREDFLAGS :=
  DOC_OUTDIR := project
endif

PROJECTPATH := ../project
DOCPATH := ../../doc

EBROWSEFILE := BROWSE
CTAGSFILE := tags
ETAGSFILE := TAGS
DOXYGENFILE_LIB := Doxyfile-libathome.conf
DOXYGENFILE := Doxyfile-$(DOC_OUTDIR).conf

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
ifeq (,$(OS_IS_WIN))
  RUN_ENV := \
    LD_LIBRARY_PATH=$(subst $(NULLCHAR) ,:,$(LD_PATHS)):$$LD_LIBRARY_PATH
else
  RUN_ENV := PATH=$(subst $(NULLCHAR) ,:,$(LD_PATHS)):$$PATH
endif

TAGEDFILES := $(wildcard *.$(CEXT) *.$(HEXT) *.$(SEXT))

CTAGSFLAGS :=
ETAGSFLAGS :=
EBROWSEFLAGS :=

# End of Variable definitions of all static stuff
# ********************************************************************
