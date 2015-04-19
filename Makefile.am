#
# Copyright (c), Recep Aslantas.
#
# MIT License (MIT), http://opensource.org/licenses/MIT
# Full license can be found in the LICENSE file
#

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -std=c++11                                             \
              -I$(top_srcdir)/include                                \
              -I$(top_srcdir)/src

SUBDIRS =

include_HEADERS = include/sig.h

lib_LTLIBRARIES = libsig.la
libsig_la_LDFLAGS = -no-undefined -version-info 1:0:0
libsig_la_SOURCES = src/sig-ctx.h                                    \
                    src/sig.cc                                       \
                    src/sig-ctx.cc
libsig_la_CXXFLAGS = $(AM_CPPFLAGS)
