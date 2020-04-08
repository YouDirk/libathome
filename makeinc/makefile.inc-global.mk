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


include $(PREFIX_ROOTDIR)/makefile.config.mk

PROJECTPATH_ROOT = $(PREFIX_ITERATEDIR)/project
LIBCOMMONPATH_ROOT = $(PREFIX_ITERATEDIR)/libathome-common
LIBCLIENTPATH_ROOT = $(PREFIX_ITERATEDIR)/libathome-client

all:

.PHONY: all run run-leakcheck debug
all run run-leakcheck debug:
	$(MAKE) -C $(PROJECTPATH_ROOT) $@

.PHONY: debug-emacs
debug-emacs:
	@$(MAKE) --no-print-directory -C $(PROJECTPATH_ROOT) $@

.PHONY: recompile
recompile: clean all

.PHONY: tags-ctags tags-etags tags-ebrowse tags-all
tags-ctags tags-etags tags-ebrowse tags-all:
	$(MAKE) -C $(LIBCOMMONPATH_ROOT) $@
	$(MAKE) -C $(LIBCLIENTPATH_ROOT) $@
	$(MAKE) -C $(PROJECTPATH_ROOT) $@

.PHONY: doc doc-view
doc doc-view:
	$(MAKE) -C $(LIBCOMMONPATH_ROOT) $@
	$(MAKE) -C $(PROJECTPATH_ROOT) $@

.PHONY: clean clean-all
clean:
	$(MAKE) -C $(LIBCOMMONPATH_ROOT) $@
	$(MAKE) -C $(LIBCLIENTPATH_ROOT) $@
	$(MAKE) -C $(PROJECTPATH_ROOT) $@
	rm -rf *.bak *~ $(CLEAN_FILES)
clean-all:
	$(MAKE) -C $(LIBCOMMONPATH_ROOT) _$@-recursive
	$(MAKE) -C $(LIBCLIENTPATH_ROOT) _$@-recursive
	$(MAKE) -C $(PROJECTPATH_ROOT) $@
	rm -rf *.bak *~ $(CLEAN_FILES) $(CLEAN_ALL_FILES)
