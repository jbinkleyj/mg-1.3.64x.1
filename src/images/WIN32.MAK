# Generated automatically from Makefile.in by configure.
###########################################################################
#
# Makefile.in -- Makefile for the MG system
# Copyright (C) 1994  Neil Sharman
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
#       @(#)Makefile.in	1.10 22 Mar 1994
#
###########################################################################

#PACKAGE = mg
#VERSION = 1.2
 
#SHELL = /bin/sh
#exec_prefix = ${prefix}
#prefix = /home/rpeeters/mg/mg-1.3.2
#srcdir = .
#subdir = src/images
#top_srcdir = ../..
#transform = s,x,x,
 
CC = cl
#CFLAGS = -g -O -Wall 
CPPFLAGS = 
DEFS = -DHAVE_CONFIG_H -D__WIN32__ -D_LITTLE_ENDIAN
#INSTALL = /usr/local/gnu/bin/install -c
#INSTALL_DATA = ${INSTALL} -m 644
#INSTALL_PROGRAM = ${INSTALL}
LDFLAGS = 
LIBS = ..\..\lib\libmg.lib 
#TOUCH = echo timestamp >
 
#bindir = $(exec_prefix)/bin
#datadir = $(prefix)/share
#libexecdir = $(exec_prefix)/libexec
#localedir = $(datadir)/locale
#mandir = $(prefix)/man/man1
 
COMPILE = $(CC) -c $(DEFS) -I. -I..\.. -I..\..\lib
LINK = $(CC) $(LDFLAGS)

.SUFFIXES:
.SUFFIXES: .c .obj
.c.obj:
	$(COMPILE) $<
 

o = .obj
e = .exe
 

SOURCES = \
  arithcode.c     extractor.c     mgbilevel.c     mgticdump.c     utils.c \
  bilevel.c       felics.c        mgfelics.c      mgticprune.c    mgticstat.c\
  codeoffsets.c   marklist.c      mgtic.c         pbmtools.c      lstevent.c \
  codesyms.c      match.c         mgticbuild.c    sortmarks.c     ppm.c  


 
HEADERS = \
  arithcode.h     codeoffsets.h   marklist.h      sortmarks.h \
  basictypes.h    codesyms.h      match.h         utils.h     \
  bilevel.h       extractor.h     pbmtools.h      model.h

MISC          = Makefile.in 

ALLFILES      =	$(SOURCES) $(HEADERS) $(MISC) $(MAN)

DISTFILES     = $(ALLFILES) 

EXEC          = mgfelics$e mgbilevel$e mgtic$e mgticbuild$e mgticprune$e mgticstat$e \
                mgticdump$e

MAN           = mgfelics.1 mgbilevel.1 mgtic.1 mgticbuild.1  mgticprune.1 \
                mgticstat.1 mgticdump.1

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


all: $(EXEC)


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

FELICS_OBJS = felics$o mgfelics$o utils$o

mgfelics$e: $(FELICS_OBJS)
	$(LINK) $(FELICS_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

BILEVEL_OBJS = \
	arithcode$o bilevel$o extractor$o marklist$o mgbilevel$o \
	pbmtools$o utils$o
            
mgbilevel$e: $(BILEVEL_OBJS)
	$(LINK) $(BILEVEL_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

TIC_OBJS = \
	mgtic$o extractor$o marklist$o pbmtools$o utils$o match$o \
	sortmarks$o codesyms$o ppm$o lstevent$o arithcode$o codeoffsets$o bilevel$o

mgtic$e: $(TIC_OBJS) 
	$(LINK) $(TIC_OBJS) $(LIBS) 

TICBUILD_OBJS = mgticbuild$o extractor$o marklist$o pbmtools$o utils$o

mgticbuild$e: $(TICBUILD_OBJS)
	$(LINK) $(TICBUILD_OBJS) $(LIBS)
 
TICPRUNE_OBJS = \
	mgticprune$o extractor$o marklist$o pbmtools$o \
	utils$o match$o

mgticprune$e: $(TICPRUNE_OBJS)
	$(LINK) $(TICPRUNE_OBJS) $(LIBS)
 
TICSTAT_OBJS = mgticstat$o extractor$o marklist$o pbmtools$o utils$o

mgticstat$e: $(TICSTAT_OBJS)
	$(LINK) $(TICSTAT_OBJS) $(LIBS) 
 
TICDUMP = mgticdump$o extractor$o marklist$o pbmtools$o utils$o

mgticdump$e: $(TICDUMP)
	$(LINK) $(TICDUMP) $(LIBS) 
 
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#ansi2knr: ansi2knr.o
#	$(LINK) ansi2knr.o $(LIBS)
#
install: execinstall maninstall
uninstall: execuninstall manuninstall

#execinstall: $(EXEC)
#	$(top_srcdir)/mkinstalldirs $(bindir); \
#	for name in $(EXEC); do \
#	  $(INSTALL_PROGRAM) $$name \
#	  $(bindir)/`echo $$name | sed '$(transform)'`; \
#	done

#execuninstall:
#	for name in $(EXEC); do \
#	 rm -f $(bindir)/`echo $$name | sed '$(transform)'`; \
#	done

maninstall: $(MAN)
	$(top_srcdir)/mkinstalldirs $(mandir); \
	for f in $(MAN); do \
	  $(INSTALL_DATA) $$f $(mandir)/$$f ;\
	done

manuninstall: $(MAN)
	for f in $(MAN); do \
	  rm -f $(mandir)/$$f ;\
	done


mostlyclean:
	rm -f *$o _*.c _*.o *._c *._o core core.*
 
clean: mostlyclean
	rm -f $(EXEC)
 
distclean: clean
	rm -f ansi2knr
	rm -f Makefile
 
maintainer-clean: distclean
	@echo "This command is intended only for maintainers to use;"
	@echo "rebuilding the deleted files may require special tools."
 
#dist: $(DISTFILES)
#	for file in $(DISTFILES); do \
#	  ln $(srcdir)/$$file $(distdir) 2> /dev/null \
#	  || cp -p $(srcdir)/$$file $(distdir); \
#	done
 
#Makefile: Makefile.in ../../config.status
#	cd ../.. && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= ./config.status
 
# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
#.NOEXPORT:
