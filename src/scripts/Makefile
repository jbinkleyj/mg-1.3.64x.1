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

PACKAGE = mg
VERSION = 1.3.64x
 
SHELL = /bin/sh

exec_prefix = ${prefix}
prefix = /home/jobin/Downloads/mg-1.3.64x.1
srcdir = .
subdir = src/scripts
top_srcdir = ../..
transform = s,x,x,
 
DEFS = -DHAVE_CONFIG_H
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
TOUCH = echo timestamp >
 
bindir = $(exec_prefix)/bin
datadir = $(prefix)/share
libexecdir = $(exec_prefix)/libexec
localedir = $(datadir)/locale
mandir = $(prefix)/man/man1
 
SHFILES       =	mgbuild.sh mgbuild_4.sh mg_get.sh xmg.sh  mg_get_merge.sh mgmerge.sh

MISC          = Makefile.in 

ALLFILES      =	$(SHFILES) $(MISC) $(MAN)

DISTFILES     = $(ALLFILES) 

EXEC          = mgbuild mg_get xmg mg_get_merge mgmerge mgbuild_4

MAN           = mgbuild.1 mgbuild_4.1 mg_get.1 xmg.1 mg_get_merge.1 mgmerge.1

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


all:	$(EXEC)


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

mgbuild : mgbuild.sh
	@rm -f $@
	cat mgbuild.sh >$@
	chmod +x $@

mgbuild_4 : mgbuild_4.sh
	@rm -f $@
	cat mgbuild_4.sh >$@
	chmod +x $@

mg_get : mg_get.sh
	@rm -f $@
	cat mg_get.sh >$@
	chmod +x $@

mgmerge : mgmerge.sh
	@rm -f $@
	cat mgmerge.sh >$@
	chmod +x $@

mg_get_merge : mg_get_merge.sh
	@rm -f $@
	cat mg_get_merge.sh >$@
	chmod +x $@

xmg : xmg.sh
	@rm -f $@
	cat xmg.sh >$@
	chmod +x $@

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


install: execinstall maninstall
uninstall: execuninstall manuninstall

execinstall: $(EXEC)
	$(top_srcdir)/mkinstalldirs $(bindir); \
	for name in $(EXEC); do \
	  $(INSTALL_PROGRAM) $$name \
	  $(bindir)/`echo $$name | sed '$(transform)'`; \
	done

execuninstall:
	for name in $(EXEC); do \
	 rm -f $(bindir)/`echo $$name | sed '$(transform)'`; \
	done

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
 
clean: mostlyclean
	rm -f $(EXEC)
 
distclean: clean
	rm -f ansi2knr
	rm -f Makefile Makefile.tmp.bak
 
maintainer-clean: distclean
	@echo "This command is intended only for maintainers to use;"
	@echo "rebuilding the deleted files may require special tools."
 
dist: $(DISTFILES)
	for file in $(DISTFILES); do \
	  ln $(srcdir)/$$file $(distdir) 2> /dev/null \
	  || cp -p $(srcdir)/$$file $(distdir); \
	done
 
Makefile: Makefile.in ../../config.status
	cd ../.. && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= ./config.status
 
# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:

depend:
	cp Makefile Makefile.tmp
	makedepend -f Makefile.tmp -w 0 -- $(COMPILEFLAGS) -- $(SOURCES)
	grep -v '[.]o: [/]' Makefile.tmp >Makefile
	rm -f Makefile.tmp

# DO NOT DELETE THIS LINE -- make depend depends on it.
