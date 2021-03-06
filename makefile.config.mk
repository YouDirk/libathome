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

# Set to 1 for debug build (recommended during development)
#
# For things such like to force `libathome_common::Log` output to
# `/dev/stdout`, enable logging backtrace at runtime error, etc ...
#
# values: [1 0]
#
DEBUG_BUILD := 1

# Version format: MAJOR.MINOR.PATCH{-SUFFIX}
# MAJOR  - Breaking, changing/removing classes, class members, functional
# MINOR  - Backward compatible, adding classes, class members
# PATCH  - Bug fixes only
# SUFFIX - (optional) -dev/-beta{X}/-rc{X}
#
VERSION_MAJOR := 0
VERSION_MINOR := 1
VERSION_PATCH := 0
VERSION_SUFFIX := -dev

# This variable has no effects on Linux/Unix-like systems.
#
# Set to 1 if you are developing on a Windows platfrom and want to
# generate PDB files.  This produces better readable backtraces in
# class Error on Windows.  (recommended)
#
# Negative side-effect: GDB (`$> make debug`) cannot load symbol
# tables anymore on Windows.  Set this variable to 0 before reompiling
# and running GDB.
#
# values: [1 0]
#
IFWIN_GENPDB := 1

# Only used if DEBUG_BUILD = 1, otherwise the best opimization level
# will be used.
#
# values: [-Og -O0 -O1 -O2 -O3 -Os -Ofast]
#
OPTFLAG := -Og

# Warning level of compiler.
#
# values: [-Wall -Wextra -Werror]
#
WARNFLAGS := -Wall -Wextra -Wformat-security

# C++ standard
#
# values: -std=[c++11 c++98 c++0x c++14 c++17]
#
CPPSTDFLAG := -std=c++11

# ********************************************************************
