ACLOCAL_AMFLAGS = -I m4
AM_CXXFLAGS = -std=c++11 -Wall -Werror
AM_CPPFLAGS = -I $(top_srcdir)/include

lib_LTLIBRARIES = libzeopp.la
libzeopp_la_SOURCES = \
	src/serial/Packet.cpp \
	src/serial/ProtocolException.cpp \
	src/serial/Reader.cpp

serialincludedir = $(includedir)/zeo/serial
serialinclude_HEADERS = \
	include/zeo/serial/Packet.hpp \
	include/zeo/serial/ProtocolException.hpp \
	include/zeo/serial/Reader.hpp

noinst_PROGRAMS = test

test_LDADD = libzeopp.la
test_SOURCES = util/test.cpp
