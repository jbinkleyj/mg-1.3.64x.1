###########################################################################
#
# Makefile.in -- Makefile for the general purpose library
# Copyright (C) 1994  Neil Sharman; (C) 1995 Tim Shimmin
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#       @(#)Makefile.in	1.3 16 Mar 1994
#
###########################################################################

PACKAGE = mg
VERSION = 1.3.64x

SHELL = /bin/sh
srcdir = .
subdir = lib


AR = ar
CC = gcc
CFLAGS = -g -O2
CPPFLAGS = 
DEFS = -DHAVE_CONFIG_H
RANLIB = ranlib

prefix = /home/jobin/Downloads/mg-1.3.64x.1
datadir = $(prefix)/share
localedir = $(datadir)/locale

COMPILEFLAGS = $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
COMPILE = $(CC) -c $(COMPILEFLAGS)

.SUFFIXES:
.SUFFIXES: .c .o
.c.o:
	$(COMPILE) $<

ANSI2KNR = 
o = .o

.SUFFIXES: ._c ._o
.c._c:
	./ansi2knr $< > $*.tmp && mv $*.tmp $@
._c._o:
	@echo $(COMPILE) $<
	@rm -f _$*.c
	@ln $< _$*.c && $(COMPILE) _$*.c && mv _$*.o $@ && rm _$*.c
.c._o:
	./ansi2knr $< > $*.tmp && mv $*.tmp $*._c
	@echo $(COMPILE) $*._c
	@rm -f _$*.c
	@ln $*._c _$*.c && $(COMPILE) _$*.c && mv _$*.o $@ && rm _$*.c

INCLUDES = -I.. -I$(srcdir)

HEADERS = \
    bitio_m_random.h   filestats.h        local_strings.h    stem.h \
    bitio_m_stdio.h    getopt.h           memlib.h \
    bitio_gen.h        bitio_mem.h        heap.h             messages.h \
    bitio_m.h          bitio_mems.h       huffman.h          timing.h \
    bitio_m_mem.h      bitio_random.h     huffman_mem.h      perf_hash.h \
    bitio_m_mems.h     bitio_stdio.h      huffman_stdio.h    sptree.h \
    regex.h            rx.h               pathmax.h          getpagesize.h \
    random.h

SOURCES = \
    bitio_random.c    heap.c            memlib.c \
    bitio_stdio.c     huffman.c         messages.c \
    bitio_gen.c       filestats.c       huffman_mem.c     perf_hash.c \
    bitio_mem.c       getopt.c          huffman_stdio.c   sptree.c \
    bitio_mems.c      getopt1.c         local_strings.c   stem.c \
    timing.c          regex.c           rx.c              \
    alloca.c          error.c           xmalloc.c         strstr.c \
    gmalloc.c         ftruncate.c       strcasecmp.c      random.c

OBJECTS =   rx$U.o \
    bitio_random$o    heap$o            memlib$o \
    bitio_stdio$o     huffman$o         messages$o \
    bitio_gen$o       filestats$o       huffman_mem$o     perf_hash$o \
    bitio_mem$o       getopt$o          huffman_stdio$o   sptree$o \
    bitio_mems$o      getopt1$o         local_strings$o   stem$o \
    timing$o          regex$o		rx$o \
    error$o           xmalloc$o \
    random$o

DISTFILES = Makefile.in $(HEADERS) $(SOURCES)

all : libmg.a

check:

libmg.a : $(OBJECTS)
	rm -f libmg.a
	$(AR) cru libmg.a $(OBJECTS)
	$(RANLIB) libmg.a || $(AR) s libmg.a

$(OBJECTS): ../config.h

install:
installdirs:
uninstall:
execinstall:
execuninstall:
maninstall:
manuninstall:

dist: $(DISTFILES)
	for file in $(DISTFILES); do \
	  ln $$file $(distdir) 2> /dev/null \
	  || cp -p $$file $(distdir); \
	done

mostlyclean:
	rm -f *.o *_.o

clean: mostlyclean
	rm -f libmg.a

distclean: clean
	rm -f Makefile Makefile.tmp.bak

maintainer-clean: distclean
	@echo "This command is intended only for maintainers to use;"
	@echo "rebuilding the deleted files may require special tools."

Makefile: Makefile.in ../config.status
	cd .. && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= ./config.status

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:

depend:
	cp Makefile Makefile.tmp
	makedepend -f Makefile.tmp -w 0 -- $(COMPILEFLAGS) -- $(SOURCES)
	grep -v '[.]o: [/]' Makefile.tmp >Makefile
	rm -f Makefile.tmp

# DO NOT DELETE THIS LINE -- make depend depends on it.

bitio_random.o: ../sysfuncs.h
bitio_random.o: ../config.h
bitio_random.o: ../mg_types.h
bitio_random.o: ./getopt.h
bitio_random.o: memlib.h
bitio_random.o: bitio_m_random.h
bitio_random.o: bitio_m.h
heap.o: heap.h
memlib.o: ../sysfuncs.h
memlib.o: ../config.h
memlib.o: ../mg_types.h
memlib.o: ./getopt.h
memlib.o: memlib.h
bitio_stdio.o: ../sysfuncs.h
bitio_stdio.o: ../config.h
bitio_stdio.o: ../mg_types.h
bitio_stdio.o: ./getopt.h
bitio_stdio.o: bitio_m_stdio.h
bitio_stdio.o: bitio_m.h
huffman.o: ../sysfuncs.h
huffman.o: ../config.h
huffman.o: ../mg_types.h
huffman.o: ./getopt.h
huffman.o: memlib.h
huffman.o: huffman.h
huffman.o: filestats.h
huffman.o: messages.h
huffman.o: netorder.h
messages.o: ../sysfuncs.h
messages.o: ../config.h
messages.o: ../mg_types.h
messages.o: ./getopt.h
messages.o: messages.h
bitio_gen.o: ../sysfuncs.h
bitio_gen.o: ../config.h
bitio_gen.o: ../mg_types.h
bitio_gen.o: ./getopt.h
bitio_gen.o: bitio_m.h
filestats.o: ../sysfuncs.h
filestats.o: ../config.h
filestats.o: ../mg_types.h
filestats.o: ./getopt.h
filestats.o: memlib.h
filestats.o: filestats.h
filestats.o: netorder.h
huffman_mem.o: ../sysfuncs.h
huffman_mem.o: ../config.h
huffman_mem.o: ../mg_types.h
huffman_mem.o: ./getopt.h
huffman_mem.o: bitio_m.h
huffman_mem.o: bitio_m_mem.h
huffman_mem.o: huffman.h
huffman_mem.o: filestats.h
perf_hash.o: ../sysfuncs.h
perf_hash.o: ../config.h
perf_hash.o: ../mg_types.h
perf_hash.o: ./getopt.h
perf_hash.o: local_strings.h
perf_hash.o: memlib.h
perf_hash.o: messages.h
perf_hash.o: perf_hash.h
perf_hash.o: netorder.h
bitio_mem.o: ../sysfuncs.h
bitio_mem.o: ../config.h
bitio_mem.o: ../mg_types.h
bitio_mem.o: ./getopt.h
bitio_mem.o: bitio_m_mem.h
bitio_mem.o: bitio_m.h
getopt.o: ../config.h
huffman_stdio.o: ../sysfuncs.h
huffman_stdio.o: ../config.h
huffman_stdio.o: ../mg_types.h
huffman_stdio.o: ./getopt.h
huffman_stdio.o: bitio_m.h
huffman_stdio.o: bitio_m_stdio.h
huffman_stdio.o: huffman.h
huffman_stdio.o: filestats.h
sptree.o: ../sysfuncs.h
sptree.o: ../config.h
sptree.o: ../mg_types.h
sptree.o: ./getopt.h
sptree.o: sptree.h
bitio_mems.o: ../sysfuncs.h
bitio_mems.o: ../config.h
bitio_mems.o: ../mg_types.h
bitio_mems.o: ./getopt.h
bitio_mems.o: bitio_m_mems.h
bitio_mems.o: bitio_m.h
getopt1.o: ../config.h
getopt1.o: ./getopt.h
local_strings.o: ../sysfuncs.h
local_strings.o: ../config.h
local_strings.o: ../mg_types.h
local_strings.o: ./getopt.h
local_strings.o: local_strings.h
local_strings.o: memlib.h
stem.o: ../sysfuncs.h
stem.o: ../config.h
stem.o: ../mg_types.h
stem.o: ./getopt.h
timing.o: ../sysfuncs.h
timing.o: ../config.h
timing.o: ../mg_types.h
timing.o: ./getopt.h
timing.o: timing.h
regex.o: ../config.h
regex.o: regex.h
rx.o: ../config.h
rx.o: rx.h
alloca.o: ../config.h
error.o: ../config.h
xmalloc.o: ../config.h
gmalloc.o: ../config.h
ftruncate.o: ../config.h
random.o: ../mg_types.h
