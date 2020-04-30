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


include ../../makefile.config.mk

include ../../.makefile.cache.mk
include ../../makeinc/makefile.check.mk

include ../../makeinc/makefile.variables.mk


# ********************************************************************
# Targets for library dirs and project dirs

.PHONY: all all-this
all:
	@for i in $(LD_PATHS); do \
	  echo $(MAKE) -C $$i all-this; \
	  $(MAKE) -C $$i all-this || exit 1; \
	done;
	$(MAKE) all-this
all-this: $(OUTPUT) ../$(MAIN_HEADER)

.PHONY: recompile
recompile: clean all

.PHONY: run run-leakcheck

# Compiling library?
ifneq (,$(LIBNAME))
run run-leakcheck:
	$(MAKE) -C $(PROJECTPATH) $@
else
run: all
	$(RUN_ENV) ./$(OUTPUT) $(ARGS)
run-leakcheck: all
  ifeq (,$(VALGRIND_OPT))
	$(error $(ERRB) VALGRIND command not found!  Try '$$> apt-get \
	  install valgrind' for installation.  Or use MSYS2 command \
	  '$$> pacman -S <package>'.  After install run \
	  '$$> make clean-all')
  else
	$(RUN_ENV) $(VALGRIND_OPT) --leak-check=full ./$(OUTPUT) $(ARGS)
  endif
endif

# Using GDB shell:
# (gdb)$> b{break} Class::method
# (gdb)$> c{continue}
# (gdb)$> wa{watch} Expression
# (gdb)$> p{print} Expression
# (gdb)$> n{next}/s{step}
# (gdb)$> q{quit}
.PHONY: debug debug-emacs

# Compiling library?
ifneq (,$(LIBNAME))
debug:
	$(MAKE) -C $(PROJECTPATH) $@
  # Must have no output until GDB is started
debug-emacs:
	@$(MAKE) --no-print-directory -C $(PROJECTPATH) $@
else
debug: all
  ifeq (,$(DEBUGGER_OPT))
	$(error $(ERRB) Debugger command not found!  Try '$$> apt-get \
	  install gdb' for installation.  Or use MSYS2 command \
	  '$$> pacman -S <package>'.  After install run \
	  '$$> make clean-all')
  else ifeq (1,$(if $(OS_IS_WIN),$(IFWIN_GENPDB),0))
	$(error $(ERRB) GDB will not be able to load symbol tables, \
	  because PDB conversation is enabled!  Set 'IFWIN_GENPDB := 0' \
	  in makefile.config.mk and try again)
  else
	$(RUN_ENV) $(DEBUGGER_OPT) $(DEBUGGERFLAGS) $(OUTPUT)
  endif
  # Must have no output until GDB is started
debug-emacs:
  ifeq (,$(DEBUGGER_OPT))
	$(error $(ERRB) Debugger command not found!  Try '$$> apt-get \
	  install gdb' for installation.  Or use MSYS2 command \
	  '$$> pacman -S <package>'.  After install run \
	  '$$> make clean-all')
  else ifeq (1,$(if $(OS_IS_WIN),$(IFWIN_GENPDB),0))
	$(error $(ERRB) GDB will not be able to load symbol tables, \
	  because PDB conversation is enabled!  Set 'IFWIN_GENPDB := 0' \
	  in makefile.config.mk, then run '$$> make' and then try again)
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

.PHONY: doc
doc: _clean_doc
ifeq (,$(DOXYGEN_OPT))
	$(error $(ERRB) DOXYGEN command not found!  Try '$$> apt-get \
	  install doxygen' for installation.  Or use MSYS2 command \
	  '$$> pacman -S <package>'.  After install run \
	  '$$> make clean-all')
else
	@echo "Generating '$(DOCPATH)/$(DOXYGENFILE)'"
  ifeq (,$(LIBNAME))
	@printf "##\n## THIS FILE WAS GENERATED!  CHANGES WILL BE$(\
	  ) OVERRIDDEN BY BUILD SYSTEM!\n##\n\n" \
	  > $(DOCPATH)/$(DOXYGENFILE); \
	$(SED) '$(\
	  )s~^\(OUTPUT_DIRECTORY\).*~\1 = $(DOC_OUTDIR)~;$(\
	  )s~\(\.\./src\)/[^ ]\+~\1/$(DOC_OUTDIR)~;$(\
	  )s~^\(STRIP_FROM_INC_PATH\).*~\1 =~;$(\
	  )s~^ \+\.\. *$$~~;$(\
	  )s~^\(PROJECT_NAME\).*~\1 = "$(PROJECT_NAME)"~;$(\
	  )s~^\(PROJECT_NUMBER\).*~\1 = $(PROJECT_VERSION)~;$(\
	  )s~^\(HTML_OUTPUT\).*~\1 = $(PROJECT_VERSION)~;$(\
	  )s~^\(PROJECT_BRIEF\).*~\1 = "$(PROJECT_BRIEF)"~;$(\
	  )s~^\(PROJECT_LOGO\).*~\1 = $(PROJECT_LOGO)~;$(\
	  )s~^\(IMAGE_PATH\).*~\1 =~;$(\
	  )s~^\(USE_MDFILE_AS_MAINPAGE\).*~\1 = $(DOXYGEN_MDFILE)~;$(\
	  )' $(DOCPATH)/$(DOXYGENFILE_LIB) >> $(DOCPATH)/$(DOXYGENFILE)
	cd $(DOCPATH) && $(DOXYGEN_OPT) -u $(DOXYGENFILE)
	cd $(DOCPATH) && $(DOXYGEN_OPT) $(DOXYGENFILE)
  else # ifeq (,$(LIBNAME))
	@$(SED) -i '$(\
	  )s~^\(PROJECT_NUMBER\).*~\1 = $(VERSION_LIB)~;$(\
	  )s~^\(HTML_OUTPUT\).*~\1 = $(VERSION_LIB)~;$(\
	  )s~^\(PROJECT_LOGO\).*~\1 = ../trunk/$(LOGO_110_55)~;$(\
	  )s~^\(HAVE_DOT\).*~\1 = $(DOXYGEN_HAVEDOT)~;$(\
	  )s~^\(GENERATE_LATEX\).*~\1 = $(DOXYGEN_HAVELATEX)~;$(\
	  )' $(DOCPATH)/$(DOXYGENFILE)
	cd $(DOCPATH) && $(DOXYGEN_OPT) -u $(DOXYGENFILE)
	cd $(DOCPATH) && $(DOXYGEN_OPT) $(DOXYGENFILE)
	@echo "Generating '$(DOC_OUTPATH)/default.css'"
	@$(SED) '$(\
	)' $(DOCPATH)/default.templ.css > $(DOC_OUTPATH)/default.css
	@echo "Generating '$(DOC_OUTPATH)/index.html'"
	@versions=$$(cd $(DOC_OUTPATH); ls -dr [0-9]*); \
	 linklist=$$(for i in $$versions; do \
	   printf "<li><a class="version_a" href=\"$$i/index.html\">$(\
	     )lib@home API $$i</a><br>API Reference Documentation</li>$(\
	     )\\\\n"; \
	   done); \
	 $(SED) '$(\
	   )s~{LINKLIST}~'"$$linklist"'~;$(\
	   )s~{PROJECT_URL}~$(WEB_PROJECT_URL)~;$(\
	   )s~{PROJECT_LINKNAME}~$(WEB_PROJECT_LINKNAME)~;$(\
	   )s~{BUGS_URL}~$(WEB_BUGS_URL)~;$(\
	   )s~{BUGS_LINKNAME}~$(WEB_BUGS_LINKNAME)~;$(\
	   )s~{PROJECTDOC_URL}~$(WEB_PROJECTDOC_URL)~;$(\
	   )s~{PROJECTDOC_LINKNAME}~$(WEB_PROJECTDOC_LINKNAME)~;$(\
	   )s~{LOGODOC_URL}~$(WEB_LOGODOC_URL)~;$(\
	 )' $(DOCPATH)/index.templ.html > $(DOC_OUTPATH)/index.html
	mkdir -p $(DOC_OUTPATH)/$(WEB_IMG_PATH)
	cp -f $(TRUNKPATH)/$(LOGO_DOC) $(DOC_OUTPATH)/$(WEB_IMG_PATH)/
	cp -f $(TRUNKPATH)/$(LOGO_FAVICON_LARGE) \
	  $(DOC_OUTPATH)/$(WEB_IMG_PATH)/
	cp -f $(TRUNKPATH)/$(LOGO_FAVICON) $(DOC_OUTPATH)/favicon.ico
  endif # ifeq (,$(LIBNAME))
endif # ifeq (,$(DOXYGEN_OPT))
.PHONY: doc-view
doc-view: doc
ifeq (,$(BROWSER_OPT))
	$(warning $(WARNB) BROWSER command not found!  Try '$$> apt-get \
	  install firefox-esr' for installation. \
	  $(NL)$(NL)      --> $(DOCHTMLPATH)/index.html$(NL))
else
  ifeq (,$(LIBNAME))
	$(BROWSER_OPT) $(DOCHTMLPATH)/index.html
  else # ifeq (,$(LIBNAME))
	$(BROWSER_OPT) $(DOC_OUTPATH)/index.html
  endif # ifeq (,$(LIBNAME))
endif

.PHONY: _clean-makecache _clean-deps _clean-tags _clean_doc clean \
        clean-all _clean-all-recursive
# _CLEAN_MAKECACHE must be the last one in the dependency list,
# because it will be regenerated during recursive CLEAN calls
.PHONY: _clean_makecache
_clean-makecache:
	-rm -f ../../.makefile.cache.mk
_clean-deps:
	-rm -f *.$(DEPEXT)
_clean-tags:
	-rm -f $(CTAGSFILE) $(ETAGSFILE) $(EBROWSEFILE)
_clean_doc:
	-rm -rf $(DOCHTMLPATH) $(DOCLATEXPATH)
	-rmdir --ignore-fail-on-non-empty $(DOCPATH)/$(DOC_OUTDIR)
clean: _clean-deps
	-rm -rf *.$(OEXT) *.$(LOGEXT) *~ $(addprefix $(DOCPATH)/,*.bak *~) \
	  $(addprefix $(TOOLSPATH)/,*.bak *~)
# Compiling library?
ifneq (,$(LIBNAME))
	$(TOUCH) $(MAIN_HEADER)
endif
clean-all: clean _clean-tags _clean_doc _clean-makecache
	-rm -rf *.$(PDBEXT) $(OUTPUT) $(TOOLSPATH)
_clean-all-recursive: clean _clean-tags _clean_doc
	-rm -rf *.$(PDBEXT) $(OUTPUT)

%.$(DEPEXT): %.$(CEXT)
	@$(MAKEDEP) $(CCFLAGS) -MQ $*.$(OEXT) -o $@ $<
%.$(DEPEXT): %.$(SEXT)
	@$(MAKEDEP) $(CCFLAGS) -MQ $*.$(OEXT) -o $@ $<

%.$(OEXT): %.$(CEXT) $(MAKEFILEZ)
	$(CC) $(CCFLAGS) -o $@ $<
%.$(OEXT): %.$(SEXT) $(MAKEFILEZ)
	$(AS) $(ASFLAGS) -o $@ $<

$(CTAGSFILE): $(TAGEDFILES)
ifeq (,$(CTAGS_OPT))
	$(error $(ERRB) CTAGS command not found!  Try '$$> apt-get \
	  install emacs-bin-common' for installation.  Or use MSYS2 \
	  command '$$> pacman -S <package>'.  After install run \
	  '$$> make clean-all')
else
	$(CTAGS_OPT) $(CTAGSFLAGS) -o $@ $^
endif
$(ETAGSFILE): $(TAGEDFILES)
ifeq (,$(ETAGS_OPT))
	$(error $(ERRB) ETAGS command not found!  Try '$$> apt-get \
	  install emacs-bin-common' for installation.  Or use MSYS2 \
	  command '$$> pacman -S <package>'.  After install run \
	  '$$> make clean-all')
else
	$(ETAGS_OPT) $(ETAGSFLAGS) -o $@ $^
endif
$(EBROWSEFILE): $(TAGEDFILES)
ifeq (,$(EBROWSE_OPT))
	$(error $(ERRB) EBROWSE command not found!  Try '$$> apt-get \
	  install emacs-bin-common' for installation.  Or use MSYS2 \
	  command '$$> pacman -S <package>'.  After install run \
	  '$$> make clean-all')
else
	$(EBROWSE_OPT) $(EBROWSEFLAGS) -o $@ $^
endif

ifeq (1,$(if $(OS_IS_WIN),$(IFWIN_GENPDB),0))
$(OUTPUT): $(OBJFILES) $(TOOLSPATH)/$(CV2PDB)
	$(LD) $(LDFLAGS) -o $@ $(OBJFILES) $(addprefix -l,$(LIBS))
	$(TOOLSPATH)/$(CV2PDB) $@ 2> /dev/null || true
else
$(OUTPUT): $(OBJFILES)
ifneq (,$(OS_IS_WIN))
	-rm -f *.$(PDBEXT)
endif
	$(LD) $(LDFLAGS) -o $@ $(OBJFILES) $(addprefix -l,$(LIBS))
endif

$(TOOLSPATH)/$(CV2PDB):
ifeq (,$(WGET_REC))
	$(warning warning: PDB files will not be generated!  It´s \
	  recommended to install '$$> apt-get install wget'.  Otherwise \
	  you can disable this warning by setting 'IFWIN_GENPDB := 0' \
	  in makefile.config.mk ...)
else
	mkdir -p $(TOOLSPATH)
	@$(WGET_REC) -nv --show-progress --progress=bar:force -O $@ \
	  $(CV2PDB_URL)
endif

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
	echo '' > $@
	$(MAKE) _CACHE_FILE=$@ _cache

# Make sure that $(CC) was set by makefile.check.mk && _CACHE_FILE
# will not generated (empty string) in the current MAKE instance,
# before generating $(DEPFILES)
ifeq (,$(if $(SED),$(_CACHE_FILE),1))
-include $(DEPFILES)
endif

# End of Targets for library dirs and project dirs
# ********************************************************************
