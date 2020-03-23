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


include $(PREFIX_ROOTDIR)/makefile.config.mk

PROJECT_DIRNAME_ROOT = $(PREFIX_ITERATEDIR)/$(PROJECT_DIRNAME)

all:

.PHONY: all run run-leakcheck debug
all run run-leakcheck debug:
	$(MAKE) -C $(PROJECT_DIRNAME_ROOT) $@

.PHONY: debug-emacs
debug-emacs:
	@$(MAKE) --no-print-directory -C $(PROJECT_DIRNAME_ROOT) $@

.PHONY: recompile
recompile: clean all

.PHONY: tags-ctags tags-etags tags-ebrowse tags-all
tags-ctags tags-etags tags-ebrowse tags-all:
	$(MAKE) -C $(PREFIX_ITERATEDIR)/libathome-common $@
	$(MAKE) -C $(PREFIX_ITERATEDIR)/libathome-client $@
	$(MAKE) -C $(PROJECT_DIRNAME_ROOT) $@

.PHONY: clean clean-all
clean:
	$(MAKE) -C $(PREFIX_ITERATEDIR)/libathome-common $@
	$(MAKE) -C $(PREFIX_ITERATEDIR)/libathome-client $@
	$(MAKE) -C $(PROJECT_DIRNAME_ROOT) $@
	rm -f *.bak *~ $(CLEAN_FILES)
clean-all:
	$(MAKE) -C $(PREFIX_ITERATEDIR)/libathome-common _$@-recursive
	$(MAKE) -C $(PREFIX_ITERATEDIR)/libathome-client _$@-recursive
	$(MAKE) -C $(PROJECT_DIRNAME_ROOT) $@
	rm -f *.bak *~ $(CLEAN_FILES)
