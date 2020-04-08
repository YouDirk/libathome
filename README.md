> lib@home, framework to develop distributed calculations.
> Copyright (C) 2020  Dirk "YouDirk" Lehmann
>
> This program is free software: you can redistribute it and/or modify
> it under the terms of the GNU Lesser General Public License as
> published by the Free Software Foundation, either version 3 of the
> License, or (at your option) any later version.
>
> This program is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
> GNU Lesser General Public License for more details.
>
> You have received a copy of the GNU Lesser General Public License
> along with this program.  If not, see <https://www.gnu.org/licenses/>.


lib@home
========

A framework to develop `*@home` projects for distributed calculations.

Currently this project is in an very early state and the lib@home
library is not really usable.  But if you are interested we have the
following resources to check the current develpoment state:

* <b>lib@home API Reference Documentation</b>  
  https://youdirk.github.io/libathome-doc/

* <b>lib@home Project on GitHub</b>  
  https://github.com/YouDirk/libathome

* <b>lib@home Bug Tracking</b>  
  https://github.com/YouDirk/libathome/issues


Make targets
------------

```make
all: \
    Compiles the current directory and all sub-directories
clean: \
    Deletes temporary files / prepare for recompilation
clean-all: \
    Deletes all files which are not under version control
debug: \
    Compiles 'all' and run 'project' in debugger (GDB)
debug-emacs: \
    Run 'project' in debugger (GDB) with Emacs support
doc: \
    Create a Doxygen documentation of the current directory
doc-view: \
    Runs 'doc' and show the resulting documantation in Browser
recompile: \
    Runs 'clean' followed by 'all'
run: \
    Make 'all' followed by running 'project'
run-leakcheck: \
    Same as 'run', but execute 'project' in Valgrind
tags-all: \
    Make 'tags-ctags', 'tags-etags' and 'tags-ebrowse'
tags-ctags: \
    Runs 'ctags' indexer for TAB auto-completion
tags-ebrowse: \
    Runs 'ebrowse' indexer for TAB auto-completion in Emacs
tags-etags: \
    Runs 'etags' indexer for TAB auto-completion
```


Playing around with lib@home
----------------------------

### Debian (Linux)

The main development is done on **Linux**.  If you are using
**Debian** (or Ubuntu) you need to have at least installed `make`,
`g++` and `libc-dev`.  All these packages are part of Debian package
`build-essential`. All other dependencies will checked by running
`make`.  So after the project was cloned run:

```shell
$> apt-get install build-essential
$> cd path/to/libathome
$> make run
```

> **Hint:** To automatically check the `make`-targets which are
>           available, it´s recommended to install the Debian package
>           `$> apt-get install bash-completion`.  After doing this,
>           open a new Bash shell run `$> make ` and use the
>           **TAB**-Key twice to list the `make`-targets on-the-fly.

### MSYS2/MinGW (Windows)

If you are using Windows you need to install `MSYS2`. After
installing, run a MSYS2-Shell and make the system up-to-date by
running

```shell
$> pacman -Syu
```

Then install at least `git`, `make` and `g++`.

```shell
$> pacman -S msys/git msys/make mingw64/mingw-w64-x86_64-gcc
```

Then after you have cloned the repository run `make` to check for
other dependencies.

```shell
$> cd path/to/libathome
$> make run
```

> **Hint:** To automatically check the `make`-targets which are
>           available, it´s recommended to install the Debian package
>           `$> apt-get install bash-completion`.  After doing this,
>           open a new Bash shell run `$> make ` and use the
>           **TAB**-Key twice to list the `make`-targets on-the-fly.
