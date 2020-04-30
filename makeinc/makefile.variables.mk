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


# ********************************************************************
# Variable definitions of all static stuff

# Not Windows! :P
ifeq (,$(OS_IS_WIN))
  SOEXT := so
  DOT_BINEXT :=
  CCDEFINES +=
  SHAREDFLAGS += -rdynamic
else
  SOEXT := dll
  DOT_BINEXT := .exe
  CCDEFINES += -DOSWIN
  SHAREDFLAGS +=
endif

# Debug build?
ifeq (1,$(DEBUG_BUILD))
  DEBUGFLAGS := -g
  CCDEFINES += -DDEBUG
else
  OPTFLAG := -Ofast
  DEBUGFLAGS := -g  # Also provide debugging symbols in productive builds
  CCDEFINES +=
endif

VERSION_LIB := \
  $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)$(VERSION_SUFFIX)

# Compiling library?
ifneq (,$(LIBNAME))
  OUTPUT := $(LIBNAME).$(SOEXT)
  MAIN_HEADER := $(LIBNAME).hpp
  FPICFLAGS := -fPIC -DPIC
  SHAREDFLAGS += -shared
  DOC_OUTDIR := libathome
  VERSION_THIS := $(VERSION_LIB)
else
  OUTPUT := $(PROJECT_EXECNAME)-client$(DOT_BINEXT)
  MAIN_HEADER :=
  FPICFLAGS :=
  SHAREDFLAGS +=
  DOC_OUTDIR := project
  VERSION_THIS := $(PROJECT_VERSION)
endif

# Does shell command 'dot' exist?
ifeq (,$(DOT_OPT))
  DOXYGEN_HAVEDOT := NO
else
  DOXYGEN_HAVEDOT := YES
endif

# Does shell command 'latex' exist?
ifeq (,$(LATEX_OPT))
  DOXYGEN_HAVELATEX := NO
else
  DOXYGEN_HAVELATEX := YES
endif

CV2PDB := cv2pdb.exe
CV2PDB_URL := https://ci.appveyor.com/api/projects/rainers/visuald$(\
  )/artifacts/cv2pdb.exe?job=Environment%3A%20os%3DVisual%20Studio$(\
  )%202015%2C%20VS%3D14%2C%20APPVEYOR_BUILD_WORKER_IMAGE%3DVisual$(\
  )%20Studio%202015

PROJECTPATH := ../project
DOCPATH := ../../doc
TOOLSPATH := ../../tools
TRUNKPATH := ../../trunk

LOGO_DOC := logo-doc.640x320.png
LOGO_110_55 := logo.110x55.png
LOGO_FAVICON := logo-square-icon.32.ico
LOGO_FAVICON_LARGE := logo-square-icon.640.png

DOC_OUTPATH := $(DOCPATH)/$(DOC_OUTDIR)
DOCHTMLPATH := $(DOC_OUTPATH)/$(VERSION_THIS)
DOCLATEXPATH := $(DOC_OUTPATH)/latex

EBROWSEFILE := BROWSE
CTAGSFILE := tags
ETAGSFILE := TAGS
DOXYGENFILE_LIB := Doxyfile-libathome.conf
DOXYGENFILE := Doxyfile-$(DOC_OUTDIR).conf

MAKEFILEZ := $(addprefix ../../makeinc/, \
  ../makefile.config.mk makefile.inc.mk makefile.inc-global.mk \
  makefile.check.mk makefile.variables.mk) Makefile ../Makefile \
  ../../Makefile

CEXT := cpp
HEXT := hpp
SEXT := S
OEXT := o
DEPEXT := d
LOGEXT := log
PDBEXT := pdb

OBJFILES := $(OBJ:=.$(OEXT))
DEPFILES := $(OBJ:=.$(DEPEXT))
HFILES := $(OBJ:=.$(HEXT))

FLAGS := $(DEBUGFLAGS) $(WARNFLAGS) $(OPTFLAG) $(CPPSTDFLAG)
CCFLAGS := $(FLAGS) -c $(FPICFLAGS) $(CCDEFINES) \
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

# ********************************************************************
# Web URL stuff

WEB_IMG_PATH := img

WEB_SITE_NAME := lib@home API Reference Documentation
WEB_SITE_URL := https://youdirk.github.io/libathome-doc

WEB_INDEX_TITLE := lib@home: API Version Index
WEB_INDEX_URL := $(WEB_SITE_URL)/

WEB_PROJECT_URL := https://github.com/YouDirk/libathome
WEB_PROJECT_LINKNAME := Project on GitHub
WEB_PROJECT_DESCR := \
  lib@home, a framework to develop *@home projects for distributed \
  calculations.

WEB_BUGS_URL := https://github.com/YouDirk/libathome/issues
WEB_BUGS_LINKNAME := Bug Tracking

WEB_PROJECTDOC_URL := https://github.com/YouDirk/libathome-doc
WEB_PROJECTDOC_LINKNAME := API Doc Version Control

WEB_FAVICON_URL := favicon.ico
WEB_FAVICON_LARGE_URL := $(WEB_IMG_PATH)/$(LOGO_FAVICON_LARGE)
WEB_LOGODOC_URL := $(WEB_IMG_PATH)/$(LOGO_DOC)

# End of Variable definitions of all static stuff
# ********************************************************************
