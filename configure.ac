#
# Copyright (c), Recep Aslantas.
#
# MIT License (MIT), http://opensource.org/licenses/MIT
# Full license can be found in the LICENSE file
#

AC_PREREQ(2.59)

AC_INIT([libsig], [1.0.0], [https://github.com/recp/libsig/issues])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/])

m4_include([m4/m4_ax_cxx_compile_stdcxx_11.m4])

AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_CXX_C_O

AC_LANG([C++])

AX_CXX_COMPILE_STDCXX_11

AC_PROG_INSTALL
AM_PROG_AR

AC_ENABLE_SHARED
AC_ENABLE_STATIC

LT_INIT

AC_SYS_LARGEFILE

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
