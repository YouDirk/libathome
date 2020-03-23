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


include ../../makefile.config.mk

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
  makefile.check.mk)

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

include ../../.makefile.cache.mk
include ../../makeinc/makefile.check.mk

MAKEDEP := $(CC) $(CCFLAGS) -M


.PHONY: all all-this
all:
	@for i in $(LD_PATHS); do \
	  echo $(MAKE) -C $$i all-this; \
	  $(MAKE) -C $$i all-this; \
	done;
	$(MAKE) all-this
all-this: $(OUTPUT) ../$(MAIN_HEADER)

.PHONY: recompile
recompile: clean all

.PHONY: run run-leakcheck

# Compiling library?
ifneq (,$(LIBNAME))
run run-leakcheck:
	$(MAKE) -C ../$(PROJECT_DIRNAME) $@
else
run: all
	$(RUN_ENV) ./$(OUTPUT) $(ARGS)
run-leakcheck: all
  ifeq (,$(VALGRIND_OPT))
	$(error $(ERRB) VALGRIND command not found!  Try '$$> apt-get \
	  install valgrind' for installation.  Or use MSYS2 command \
	  '$$> pacman -S <package>')
  else
	$(RUN_ENV) $(VALGRIND_OPT) --leak-check=full ./$(OUTPUT) $(ARGS)
  endif
endif

# Using GDB shell:
# (gdb)$> b{break} Class::method
# (gdb)$> c{continue}
# (gdb)$> wa{watch} Expression
# (gdb)$> n{next}/s{step}
.PHONY: debug debug-emacs

# Compiling library?
ifneq (,$(LIBNAME))
debug:
	$(MAKE) -C ../$(PROJECT_DIRNAME) $@
  # Must have no output until GDB is started
debug-emacs:
	@$(MAKE) --no-print-directory -C ../$(PROJECT_DIRNAME) $@
else
debug: all
  ifeq (,$(DEBUGGER_OPT))
	$(error $(ERRB) Debugger command not found!  Try '$$> apt-get \
	  install gdb' for installation.  Or use MSYS2 command \
	  '$$> pacman -S <package>')
  else
	$(RUN_ENV) $(DEBUGGER_OPT) $(DEBUGGERFLAGS) $(OUTPUT)
  endif
  # Must have no output until GDB is started
debug-emacs:
  ifeq (,$(DEBUGGER_OPT))
	$(error $(ERRB) Debugger command not found!  Try '$$> apt-get \
	  install gdb' for installation.  Or use MSYS2 command \
	  '$$> pacman -S <package>')
  else
	@$(RUN_ENV) $(DEBUGGER_OPT) -i=mi $(DEBUGGERFLAGS) $(OUTPUT)
  endif
endif # ifneq (,$(LIBNAME))

.PHONY: tags-ctags tags-etags tags-ebrowse
tags-ctags: $(CTAGSFILE)
tags-etags: $(ETAGSFILE)
tags-ebrowse: $(EBROWSEFILE)

.PHONY: tags-all
tags-all: tags-ctags tags-etags tags-ebrowse

.PHONY: _clean-makecache _clean-deps _clean-tags clean clean-all \
        _clean-all-recursive
# _CLEAN_MAKECACHE must be the last one in the dependency list,
# because it will be regenerated during recursive CLEAN calls
.PHONY: _clean_makecache
_clean-makecache:
	-rm -f ../../.makefile.cache.mk
_clean-deps:
	-rm -f *.$(DEPEXT)
_clean-tags:
	-rm -f $(CTAGSFILE) $(ETAGSFILE) $(EBROWSEFILE)
clean: _clean-deps
	-rm -rf *.$(OEXT) *.$(LOGEXT) *~
# Compiling library?
ifneq (,$(LIBNAME))
	$(TOUCH) $(MAIN_HEADER)
endif
clean-all: clean _clean-tags _clean-makecache
	-rm -f $(OUTPUT)
_clean-all-recursive: clean _clean-tags
	-rm -f $(OUTPUT)

%.$(DEPEXT): %.$(CEXT)
	@$(MAKEDEP) -MQ $*.$(OEXT) -o $@ $<
%.$(DEPEXT): %.$(SEXT)
	@$(MAKEDEP) -MQ $*.$(OEXT) -o $@ $<

%.$(OEXT): %.$(CEXT) $(MAKEFILEZ)
	$(CC) $(CCFLAGS) -o $@ $<
%.$(OEXT): %.$(SEXT) $(MAKEFILEZ)
	$(AS) $(ASFLAGS) -o $@ $<

$(CTAGSFILE): $(TAGEDFILES)
ifeq (,$(CTAGS_OPT))
	$(error $(ERRB) CTAGS command not found!  Try '$$> apt-get \
	  install emacs-bin-common' for installation.  Or use MSYS2 \
	  command '$$> pacman -S <package>')
else
	$(CTAGS_OPT) $(CTAGSFLAGS) -o $@ $^
endif
$(ETAGSFILE): $(TAGEDFILES)
ifeq (,$(ETAGS_OPT))
	$(error $(ERRB) ETAGS command not found!  Try '$$> apt-get \
	  install emacs-bin-common' for installation.  Or use MSYS2 \
	  command '$$> pacman -S <package>')
else
	$(ETAGS_OPT) $(ETAGSFLAGS) -o $@ $^
endif
$(EBROWSEFILE): $(TAGEDFILES)
ifeq (,$(EBROWSE_OPT))
	$(error $(ERRB) EBROWSE command not found!  Try '$$> apt-get \
	  install emacs-bin-common' for installation.  Or use MSYS2 \
	  command '$$> pacman -S <package>')
else
	$(EBROWSE_OPT) $(EBROWSEFLAGS) -o $@ $^
endif

$(OUTPUT): $(OBJFILES)
	$(LD) $(LDFLAGS) -o $@ $^ $(addprefix -l,$(LIBS))

# Compiling library?
ifneq (,$(LIBNAME))
../$(MAIN_HEADER): $(MAIN_HEADER) $(HFILES)
	@echo "Generating $@ from $<"
	@$(SED) \
	  's/^#error.*$$/$(patsubst %,\n#include "$(LIBNAME)\/%",$^)/' \
	  $< > $@
endif

.PHONY: _cache
_cache:
../../.makefile.cache.mk: $(MAKEFILEZ)
	-rm -f $@
	$(MAKE) _CACHE_FILE=$@ _cache

-include $(DEPFILES)
