# Main Makefile for GNU tar.
# Copyright (C) 1994, 1995 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

PACKAGE = mg
VERSION = 1.3.64x


SHELL = /bin/sh

exec_prefix = ${prefix}
prefix = /home/jobin/Downloads/mg-1.3.64x.1
srcdir = .

CC = gcc
CFLAGS = -g -O2
CPPFLAGS = 
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
LDFLAGS = 
LIBS = -L$(prefix)/lib -lreadline -ltermcap -lm 
RM = rm -f
TOUCH = echo timestamp >

bindir = $(exec_prefix)/bin
infodir = $(prefix)/info
libexecdir = $(exec_prefix)/libexec

MDEFINES = 

SUBDIRS = lib src/text src/images src/scripts

.SUFFIXES:

all install uninstall execinstall execuninstall maninstall manuninstall: config.h
	for subdir in $(SUBDIRS); do \
	  echo making $@ in $$subdir; \
	  (cd $$subdir && $(MAKE) $(MDEFINES) $@) || exit 1; \
	done

# Have not written any texinfo files for the documentation yet
#info dvi:
#	cd doc && $(MAKE) $@

check: all clean-test
	echo making $@ in ./test directory
	(cd ./test && $(MAKE) all) || exit 1
	./test/testmg `pwd`

mostlyclean: mostlyclean-recursive mostlyclean-local

clean: clean-recursive clean-local clean-test

clean-local:
	-$(RM) core a.out *~ \#*

clean-test:
	(cd test; $(MAKE) clean)

distclean: distclean-recursive distclean-local
	-$(RM) config.status
	-$(RM) -r ./autom4te.cache

maintainer-clean: maintainer-clean-recursive maintainer-clean-local
	-$(RM) config.status

mostlyclean-recursive clean-recursive distclean-recursive \
maintainer-clean-recursive:
	for subdir in $(SUBDIRS); do \
	  target=`echo $@ | sed 's/-recursive//'`; \
	  echo making $$target in $$subdir; \
	  (cd $$subdir && $(MAKE) $$target) || exit 1; \
	done

mostlyclean-local:	clean-local

distclean-local: mostlyclean-local
	-$(RM) -f Makefile  Makefile.bak config.cache config.h config.log stamp-h

maintainer-clean-local: distclean-local
	@echo "This command is intended only for maintainers to use;"
	@echo "rebuilding the deleted files may require special tools."

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

distdir = $(PACKAGE)-$(VERSION)

distname = $(distdir)

DISTFILES = \
INSTALL.mg INSTALL README README.port COPYING MODIFICATIONS about_mg.html \
Makefile.in acconfig.h aclocal.m4 configure.in install-sh mkinstalldirs \
config.guess config.sub \
config.h.in configure stamp-h.in sysfuncs.h

dist: $(DISTFILES)
#-----------------------------------------------------------------
	#create distribution directory
	-rm -rf $(distdir)
	mkdir $(distdir)
#
	#link in local distribution files
	ln $(DISTFILES) $(distdir)
#
	#link in SampleData
	mkdir $(distdir)/SampleData
	(cd SampleData; $(MAKE) "distdir=../$(distdir)/SampleData" dist)
	# link in davinci stuff
	mkdir $(distdir)/SampleData/davinci
	(cd SampleData/davinci; \
	$(MAKE) "distdir=../../$(distdir)/SampleData/davinci" dist)
	# link in bible stuff
	mkdir $(distdir)/SampleData/bible
	(cd SampleData/bible; \
	$(MAKE) "distdir=../../$(distdir)/SampleData/bible" dist)
#
	#link in tests
	mkdir $(distdir)/test
	(cd test; $(MAKE) "distdir=../$(distdir)/test" dist)
#
	#link in docs directory
	mkdir $(distdir)/docs
	(cd docs; $(MAKE) "distdir=../$(distdir)/docs" dist)
#
	#link in lib directory
	mkdir $(distdir)/lib
	(cd lib; $(MAKE) "distdir=../$(distdir)/lib" dist)
#
	#link in src distribution files
	mkdir $(distdir)/src
	for dir in src/text src/images src/scripts; do \
	  echo makeing $@ in $$dir ; \
	  mkdir $(distdir)/$$dir; \
	  (cd $$dir; $(MAKE) "distdir=../../$(distdir)/$$dir" $@) ; \
	done
#
	# archive and compress
	tar -chf - $(distdir) | gzip -9 > $(distname).tar.gz
	tar -chf - $(distdir) | compress > $(distname).tar.Z
#
	# cleanup
	-$(RM) -rf $(distdir)
#-----------------------------------------------------------------

# For an explanation of the following Makefile rules, see node
# `Automatic Remaking' in GNU Autoconf documentation.
Makefile: Makefile.in config.status
	CONFIG_FILES=$@ CONFIG_HEADERS= ./config.status
config.status: configure
	./config.status --recheck
configure: configure.in aclocal.m4
	cd $(srcdir) && autoconf

config.h: stamp-h
stamp-h: config.h.in config.status
	CONFIG_FILES= CONFIG_HEADERS=config.h ./config.status
config.h.in: stamp-h.in
stamp-h.in: configure.in aclocal.m4 acconfig.h
	cd $(srcdir) && autoheader
	$(TOUCH) $(srcdir)/stamp-h.in

# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:

depend:
	for dir in $(SUBDIRS) ; \
	do \
	  echo making $@ in $$dir ; \
	  (cd $$dir; $(MAKE) CFLAGS='$(CFLAGS)' $@ ) ; \
	done

# DO NOT DELETE THIS LINE -- make depend depends on it.
