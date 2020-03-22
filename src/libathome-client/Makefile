# `lib@home`, a distributed calculations framework
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


LIBNAME = libathome-client
OBJ = Init

OUTPUT = $(LIBNAME).so
MAIN_HEADER = $(LIBNAME).hpp

DEBUGFLAGS = -g
OPTFLAG = -O0

INCLUDE_PATHS = ..
LD_PATHS = ../libathome-common
LIBS = athome-common

EBROWSEFILE = BROWSE
CTAGSFILE = tags
ETAGSFILE = TAGS
MAKEFILE = Makefile

CC = g++
AS = g++
LD = g++
DEBUGGER = gdb
VALGRIND = valgrind
EBROWSE = ebrowse
CTAGS = ctags
ETAGS = etags
SED = sed
TOUCH = touch

CEXT = cpp
HEXT = hpp
SEXT = S
OEXT = o
DEPEXT = d
LOGEXT = log

OBJFILES = $(OBJ:=.$(OEXT))
DEPFILES = $(OBJ:=.$(DEPEXT))
HFILES = $(OBJ:=.$(HEXT))

FLAGS = $(DEBUGFLAGS) -Wall $(OPTFLAG)
CCFLAGS = $(FLAGS) -c -fPIC -DPIC \
          -DDEBUG $(addprefix -I,$(INCLUDE_PATHS))
ASFLAGS = $(CCFLAGS)
LDFLAGS = $(FLAGS) \
          -shared $(addprefix -L,$(LD_PATHS))
DEBUGGERFLAGS = --quiet

NULLCHAR =
RUN_ENV = \
  LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(subst $(NULLCHAR) ,:,$(LD_PATHS))

TAGEDFILES = $(wildcard *.$(CEXT) *.$(HEXT) *.$(SEXT))

CTAGSFLAGS =
ETAGSFLAGS =
EBROWSEFLAGS =

MAKEDEP = $(CC) $(CCFLAGS) -M

.PHONY: all recompile run debug debug-emacs ctags etags ebrowse \
        all-tags clean-deps clean-tags clean clean-all all-this

all:
	@for i in $(LD_PATHS); do \
	  echo $(MAKE) -C $$i all-this; \
	  $(MAKE) -C $$i all-this; \
	done;
	$(MAKE) all-this
all-this: $(OUTPUT) ../$(MAIN_HEADER)

recompile: clean all

run: all
	$(RUN_ENV) ./$(OUTPUT) $(ARGS)
run-leakcheck: all
	$(RUN_ENV) $(VALGRIND) --leak-check=full ./$(OUTPUT) $(ARGS)

debug: all
	$(RUN_ENV) $(DEBUGGER) $(DEBUGGERFLAGS) $(OUTPUT)

# Set in GDB shell: 'set environment LD_LIBRARY_PATH=../lib'
debug-emacs: all
	$(RUN_ENV) $(DEBUGGER) -i=mi $(DEBUGGERFLAGS) $(OUTPUT)

ctags: $(CTAGSFILE)

etags: $(ETAGSFILE)

ebrowse: $(EBROWSEFILE)

all-tags: ctags etags ebrowse

clean-deps:
	rm -f *.$(DEPEXT)

clean-tags:
	rm -f $(CTAGSFILE) $(ETAGSFILE) $(EBROWSEFILE)

clean: clean-deps
	rm -rf *.$(OEXT) *.$(LOGEXT) *~
	$(TOUCH) $(MAIN_HEADER)

clean-all: clean clean-tags
	rm -f $(OUTPUT) ../$(MAIN_HEADER)

%.$(DEPEXT): %.$(CEXT)
	@$(MAKEDEP) -MQ $*.$(OEXT) -o $@ $<
%.$(DEPEXT): %.$(SEXT)
	@$(MAKEDEP) -MQ $*.$(OEXT) -o $@ $<

%.$(OEXT): %.$(CEXT) $(MAKEFILE)
	$(CC) $(CCFLAGS) -o $@ $<
%.$(OEXT): %.$(SEXT) $(MAKEFILE)
	$(AS) $(ASFLAGS) -o $@ $<

$(CTAGSFILE): $(TAGEDFILES)
	$(CTAGS) $(CTAGSFLAGS) -o $@ $^

$(ETAGSFILE): $(TAGEDFILES)
	$(ETAGS) $(ETAGSFLAGS) -o $@ $^

$(EBROWSEFILE): $(TAGEDFILES)
	$(EBROWSE) $(EBROWSEFLAGS) -o $@ $^

$(OUTPUT): $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $^ $(addprefix -l,$(LIBS))

../$(MAIN_HEADER): $(MAIN_HEADER) $(HFILES)
	@echo "Generating $@ from $<"
	@$(SED) \
	  's/^#error.*$$/$(patsubst %,\n#include "$(LIBNAME)\/%",$^)/' \
	  $< > $@

-include $(DEPFILES)
