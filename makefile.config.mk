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

# Set to 1 if debug build (recommended during development)
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
VERSION_MAJOR = 0
VERSION_MINOR = 1
VERSION_PATCH = 0
VERSION_SUFFIX = -dev

# Only used if DEBUG_BUILD = 1, otherwise best opimization level is
# used.
#
# values: [-Og -O0 -O1 -O2 -O3 -Os -Ofast]
#
OPTFLAG := -Og

# Warning level of compiler.
#
# values: [-Wall -Wextra -Werror]
#
WARNFLAGS := -Wextra

# ********************************************************************
