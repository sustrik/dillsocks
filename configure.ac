#
# Copyright (c) 2013 Luca Barbato
# Copyright (c) 2015 Martin Sustrik  All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom
# the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

################################################################################
#  Start the configuration phase.                                              #
################################################################################

AC_PREREQ([2.53])

AC_INIT([dsock], [m4_esyscmd([./package_version.sh])],
   [libmill@freelists.org], [dsock], [http://libdill.org/])
AC_CONFIG_SRCDIR([dsock.pc.in])
AM_INIT_AUTOMAKE([1.6 foreign subdir-objects tar-ustar])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CANONICAL_HOST

################################################################################
#  Retrieve the versions.                                                      #
################################################################################

AC_PROG_SED
AC_PROG_AWK

DSOCK_ABI_VERSION=m4_esyscmd([./abi_version.sh])
DSOCK_PACKAGE_VERSION=m4_esyscmd([./package_version.sh])
DSOCK_LIBTOOL_VERSION=m4_esyscmd([./abi_version.sh -libtool])

AC_SUBST(DSOCK_ABI_VERSION)
AC_SUBST(DSOCK_PACKAGE_VERSION)
AC_SUBST(DSOCK_LIBTOOL_VERSION)

AC_MSG_NOTICE([Millsocks package version: $DSOCK_PACKAGE_VERSION])
AC_MSG_NOTICE([Millsocks ABI version: $DSOCK_ABI_VERSION])

################################################################################
#  Check the compilers.                                                        #
################################################################################

AC_PROG_CC_C99
AM_PROG_CC_C_O

################################################################################
#  --enable-debug                                                              #
################################################################################

AC_ARG_ENABLE([debug], [AS_HELP_STRING([--enable-debug],
    [Enable debugging information [default=no]])])

if test "x$enable_debug" = "xyes"; then
    # Override original optimisation level - last option specified wins.
    CFLAGS="$CFLAGS -g -O0"
fi

################################################################################
#  --enable-asan                                                               #
################################################################################

dnl Enable Address Sanitizer, if supported by gcc (4.8+) or clang.
dnl http://clang.llvm.org/docs/AddressSanitizer.html
dnl https://code.google.com/p/address-sanitizer/wiki/HowToBuild
AC_ARG_ENABLE([asan],
    [AS_HELP_STRING([--enable-asan], [Enable Address Sanitizer [default=no]])],
    [enable_asan=$enableval], [enable_asan=no])
  AS_IF([test "x$enable_asan" != xno], [
    AX_CHECK_COMPILE_FLAG([-fsanitize=address],
                          [CFLAGS="$CFLAGS -fsanitize=address"],
                          [enable_asan=no])
    AS_IF([test "x$enable_asan" = xno], [
        AC_MSG_FAILURE(
            [--enable-asan is given, but not supported on this platform.
 Check out https://code.google.com/p/address-sanitizer/wiki/HowToBuild])])
    dnl Keep error messages nice. Also consider:
    dnl export ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer
    dnl export ASAN_OPTIONS=symbolize=1
    AX_CHECK_COMPILE_FLAG([-fno-omit-frame-pointer],
                          [CFLAGS="$CFLAGS -fno-omit-frame-pointer"])
    dnl AddressSanitizer symbols might not link if built with shared libs
    AC_DISABLE_SHARED
  ])

################################################################################
#  Feature checks.                                                             #
################################################################################

AC_CHECK_LIB([dill], [dill_prologue])
AC_CHECK_FUNCS([dill_prologue])

# SunOS has sockets in a separate library.
AC_CHECK_LIB([socket], [socket])

################################################################################
#  Libtool                                                                     #
################################################################################

LT_INIT

################################################################################
#  Finish the configuration phase.                                             #
################################################################################

AC_CONFIG_MACRO_DIR([m4])

AC_OUTPUT([Makefile dsock.pc])
cp confdefs.h config.h

