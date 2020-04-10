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
library is not really usable for productive stuff.  But if you are
interested in it, we provide the following resources to check the
current progress of development

* <b>lib@home API Reference Documentation</b>  
  https://youdirk.github.io/libathome-doc/

* <b>lib@home Project on GitHub</b>  
  https://github.com/YouDirk/libathome

* <b>lib@home Bug Tracking</b>  
  https://github.com/YouDirk/libathome/issues


Make targets
------------

```make
all <default>: Compiles the current directory and all sub-directories
clean:         Deletes temporary files / prepare for recompilation
clean-all:     Deletes all files which are not under version control
debug:         Compiles 'all' and runs 'project' in debugger (GDB)
debug-emacs:   Run 'project' in debugger (GDB) with Emacs support
doc:           Create a Doxygen documentation of the current directory
doc-view:      Runs 'doc' and show the resulting documentation in Browser
recompile:     Runs 'clean' followed by 'all'
run:           Make 'all' followed by running 'project'
run-leakcheck: Same as 'run', but execute 'project' in Valgrind
tags-all:      Make 'tags-ctags', 'tags-etags' and 'tags-ebrowse'
tags-ctags:    Runs 'ctags' indexer for TAB auto-completion
tags-ebrowse:  Runs 'ebrowse' indexer for TAB auto-completion in Emacs
tags-etags:    Runs 'etags' indexer for TAB auto-completion
```

Playing around with lib@home
----------------------------

### Debian (Linux)

The main development is done on **Linux**.  If you are using
**Debian** (or Ubuntu) you need to have installed at least `make`,
`g++` and `libc-dev`, these packages are part of Debian package
`build-essential`.  Also you need to install `git`.  If you are
finished and have cloned the repository to your local machine then run
`make` to check for other dependencies.

```shell
$> sudo apt-get install git build-essential
$> git clone https://github.com/YouDirk/libathome libathome
$> cd libathome
libathome$> make run
```

> **Hint:** To automatically check the `make`-targets which are
>           available on-the-fly using **TAB auto-completion**, it is
>           recommended to install the Debian package `$> apt-get
>           install bash-completion`.  After doing this, open a new
>           Bash shell, type `libathome$> make ` and use the TAB-key
>           twice to list the `make`-targets.

### MSYS2/MinGW (Windows)

If you are using Windows you need to install `MSYS2`
(https://www.msys2.org). After installation is finished, run a
MSYS2-shell and make the system up-to-date by running

```shell
$> pacman -Syu
```

Then install at least the following MSYS2/MinGW packages: `git`,
`make` and `g++`.  If you are finished and have cloned the repository
to your local machine then run `make` to check for other dependencies.


```shell
$> pacman -S msys/git msys/make mingw64/mingw-w64-x86_64-gcc
$> git clone https://github.com/YouDirk/libathome libathome
$> cd libathome
libathome$> make run
```

> **Hint:** To automatically check the `make`-targets which are
>           available on-the-fly using **TAB auto-completion**, it is
>           recommended to install the MSYS2 package `$> pacman -S
>           msys/bash-completion`.  After doing this, open a new Bash
>           shell, type `libathome$> make ` and use the TAB-key twice
>           to list the `make`-targets.
