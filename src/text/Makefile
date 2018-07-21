###########################################################################
#
# Makefile.in -- Makefile for the MG system
# Copyright (C) 1994  Neil Sharman;  1995 Tim Shimmin
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
subdir = src/text
top_srcdir = ../..
transform = s,x,x,

CC = gcc
CFLAGS = -g -O2 -lreadline -lhistory -lncurses -lreadline -lhistory -lncurses
CPPFLAGS = 

##########################################
# DEFS = -DHAVE_CONFIG_H
# Uncomment for compiling for the nzdl project
DEFS = -DHAVE_CONFIG_H -DNZDL
##########################################

INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_PROGRAM = ${INSTALL}
LDFLAGS = 
LIBMG = ../../lib/libmg.a
LIBS = -L$(prefix)/lib $(LIBMG) $(LIBREADLINE) $(LIBTERMCAP) -lreadline -ltermcap -lm 

# These two variables allow make-time overriding of configure settings:
LIBREADLINE =
LIBTERMCAP = 

TOUCH = echo timestamp >
AWK = gawk

bindir = $(exec_prefix)/bin
datadir = $(prefix)/share
libexecdir = $(exec_prefix)/libexec
localedir = $(datadir)/locale
mandir = $(prefix)/man/man1

COMPILEFLAGS = $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) $(TREC_MODE) $(READLINE)
COMPILE = $(CC) -c $(COMPILEFLAGS)
LINK = $(CC) $(COMPILEFLAGS) $(LDFLAGS) -o $@

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

INCLUDES = -I../.. -I../../lib

#############################################################################
#
# When working with the trec collection this should be uncommented.
#
# TREC_MODE = -DTREC_MODE
#
#
#############################################################################

SOURCES = \
  backend.c               locallib.c              mg_weights_build.c   \
  bool_optimiser.c        mg.special.c            mgdictlist.c         \
  bool_parser.c           mg_compression_dict.c   mgquery.c            \
  bool_query.c            mg_errors.c             mgstat.c             \
  bool_tester.c           mg_fast_comp_dict.c     mg_decompress_text.c \
  bool_tree.c             mg_files.c              query.docnums.c      \
  commands.c              mg_hilite_words.c       query.ranked.c       \
  comp_dict.c             mg_invf_dict.c          read_line.c          \
  environment.c           mg_invf_dump.c          stem_search.c        \
  mg_invf_merge.c         stemmer.c               mg_stem_idx.c        \
  mg_invf_rebuild.c       term_lists.c            mgstemidxlist.c      \
  invf_get.c              mg_passes.c             text.pass1.c         \
  ivf.pass1.c             mg_perf_hash_build.c    text.pass2.c         \
  ivf.pass2.c             mg_text_estimate.c      text_get.c           \
  lists.c                 mg_text_merge.c         weights.c            \
  query_term_list.c

HEADERS = \
  backend.h          conditions.h       locallib.h         term_lists.h \
  bool_optimiser.h   environment.h      mg.h               text.h       \
  bool_parser.h      globals.h          mg_errors.h        text_get.h   \
  bool_query.h       hash.h             mg_files.h         warranty.h   \
  bool_tree.h        help.mg.h          mg_merge.h         weights.h    \
  build.h            invf.h             read_line.h        words.h      \
  commands.h         invf_get.h         stem_search.h                   \
  comp_dict.h        lists.h            stemmer.h                       \
  query_term_list.h

MISC          = Makefile.in help.mg.src bool_parser.y warranty.src conditions.src

ALLFILES      =	$(SOURCES) $(HEADERS) $(MISC) $(MAN)

DISTFILES     = $(ALLFILES)

EXEC          = mgquery mg_weights_build mgstat mg_invf_dump mg_invf_dict \
                mg_invf_rebuild mgdictlist mg_passes mg_perf_hash_build \
                mg_compression_dict mg_text_estimate mg_fast_comp_dict \
                mg_hilite_words mg_invf_merge mg_text_merge bool_tester \
                mg_stem_idx mgstemidxlist mg_decompress_text

MAN           = mgquery.1 mg_weights_build.1 mgstat.1 mg_invf_dump.1 mg_invf_dict.1 \
                mg_invf_rebuild.1 mgdictlist.1 mg_passes.1 mg_perf_hash_build.1 \
                mg_compression_dict.1 mg_text_estimate.1 mg_fast_comp_dict.1 \
                mg_hilite_words.1 mg_invf_merge.1 mg_text_merge.1 \
                mg_stem_idx.1 mgstemidxlist.1 mg_decompress_text.1

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

all:	$(EXEC)

check:

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

help.mg.h : help.mg.src
	sed -e 's/\\/\\\\/g' -e 's/\"/\\\"/g' help.mg.src | $(AWK) '/^#/ { next; };{ if (x == 0) printf("static char *help_str[] = {\n\"%s\\n\"", $$0); else printf(",\n\"%s\\n\"", $$0); x = 1; }; END { printf("};\n"); }' - >help.mg.h

warranty.h : warranty.src
	sed  -e 's/\\/\\\\/g' -e 's/\"/\\\"/g' warranty.src | $(AWK) '/^#/ { next; };{ if (x == 0) printf("static char *warranty_str[] = {\n\"%s\\n\"", $$0); else printf(",\n\"%s\\n\"", $$0); x = 1; }; END { printf("};\n"); }' - >warranty.h

conditions.h : conditions.src
	sed -e 's/\\/\\\\/g' -e 's/\"/\\\"/g' conditions.src | $(AWK) '/^#/ { next; };{ if (x == 0) printf("static char *cond_str[] = {\n\"%s\\n\"", $$0); else printf(",\n\"%s\\n\"", $$0); x = 1; }; END { printf("};\n"); }' - >conditions.h

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

HILITE_OBJS = mg_hilite_words$o stemmer$o locallib$o

mg_hilite_words: $(HILITE_OBJS) $(LIBMG)
	$(LINK) $(HILITE_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

bool_parser.c: bool_parser.y
	$(YACC) $?
	mv y.tab.c $@

BOOL_OBJS = bool_tree$o bool_parser$o bool_optimiser$o bool_tester$o \
            term_lists$o stemmer$o stem_search$o mg_errors$o query_term_list$o

bool_tester: $(BOOL_OBJS) $(LIBMG)
	$(LINK) $(BOOL_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

QUERY_OBJS = \
	mgquery$o locallib$o lists$o \
	query.ranked$o query.docnums$o stem_search$o \
	environment$o commands$o weights$o text_get$o stemmer$o \
	read_line$o mg_errors$o backend$o invf_get$o \
	term_lists$o bool_tree$o bool_parser$o bool_optimiser$o bool_query$o \
	query_term_list$o

mgquery: $(QUERY_OBJS) $(LIBMG)
	$(LINK) $(READLINE) $(QUERY_OBJS) $(READLINE_LIBS) $(LIBS)

commands$o : help.mg.h warranty.h conditions.h

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

PASSES_OBJS = \
	mg_passes$o text.pass1$o comp_dict$o stemmer$o \
	text.pass2$o locallib$o \
	ivf.pass1$o ivf.pass2$o mg.special$o mg_files$o

mg_passes: $(PASSES_OBJS) $(LIBMG)
	$(LINK) $(PASSES_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

WEIGHTS_BUILD_OBJS = mg_weights_build$o mg_files$o

mg_weights_build: $(WEIGHTS_BUILD_OBJS) $(LIBMG)
	$(LINK) $(WEIGHTS_BUILD_OBJS) $(LIBS)

COMP_DICT_OBJS = mg_compression_dict$o mg_files$o locallib$o

mg_compression_dict: $(COMP_DICT_OBJS) $(LIBMG)
	$(LINK) $(COMP_DICT_OBJS) $(LIBS)

FAST_COMP_OBJS = mg_fast_comp_dict$o mg_files$o locallib$o

mg_fast_comp_dict: $(FAST_COMP_OBJS) $(LIBMG)
	$(LINK) $(FAST_COMP_OBJS) $(LIBS)

TEXT_EST_OBJS = mg_text_estimate$o locallib$o comp_dict$o mg_files$o

mg_text_estimate: $(TEXT_EST_OBJS) $(LIBMG)
	$(LINK) $(TEXT_EST_OBJS) $(LIBS)

STAT_OBJS = mgstat$o mg_files$o locallib$o

mgstat:	$(STAT_OBJS) $(LIBMG)
	$(LINK) $(STAT_OBJS) $(LIBS)

INVF_DUMP_OBJS = mg_invf_dump$o locallib$o mg_files$o

mg_invf_dump: $(INVF_DUMP_OBJS) $(LIBMG)
	$(LINK) $(INVF_DUMP_OBJS) $(LIBS)

INVF_DICT_OBJS = mg_invf_dict$o mg_files$o locallib$o

mg_invf_dict: $(INVF_DICT_OBJS) $(LIBMG)
	$(LINK) $(INVF_DICT_OBJS) $(LIBS)

INVF_REB_OBJS = mg_invf_rebuild$o locallib$o

mg_invf_rebuild: $(INVF_REB_OBJS) $(LIBMG)
	$(LINK) $(INVF_REB_OBJS) $(LIBS)

DICTLIST_OBJS = mgdictlist$o locallib$o

mgdictlist: $(DICTLIST_OBJS) $(LIBMG)
	$(LINK) $(DICTLIST_OBJS) $(LIBS)

PERF_HASH_OBJS = mg_perf_hash_build$o mg_files$o

mg_perf_hash_build: $(PERF_HASH_OBJS) $(LIBMG)
	$(LINK) $(PERF_HASH_OBJS) $(LIBS)

STEM_IDX_OBJS = \
                mg_stem_idx$o mg_files$o stemmer$o locallib$o \
                mg_errors$o term_lists$o

mg_stem_idx: $(STEM_IDX_OBJS) $(LIBMG)
	$(LINK) $(STEM_IDX_OBJS) $(LIBS)

STEM_IDX_LIST_OBJS = mgstemidxlist$o mg_files$o

mgstemidxlist: $(STEM_IDX_LIST_OBJS) $(LIBMG)
	$(LINK) $(STEM_IDX_LIST_OBJS) $(LIBS)

DECOMPRESS_TEXT_OBJS = \
                mg_decompress_text.o mg_files.o mg_errors.o \
                text_get.o locallib.o backend.o stem_search.o \
                term_lists.o stemmer.o weights.o invf_get.o lists.o

mg_decompress_text: $(DECOMPRESS_TEXT_OBJS) $(LIBMG)
	$(LINK) $(DECOMPRESS_TEXT_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

TEXT_MERGE_OBJS = mg_text_merge$o mg_files$o locallib$o

mg_text_merge: $(TEXT_MERGE_OBJS) $(LIBMG)
	$(LINK) $(TEXT_MERGE_OBJS) $(LIBS)

INVF_MERGE_OBJS = mg_invf_merge$o mg_files$o locallib$o

mg_invf_merge: $(INVF_MERGE_OBJS) $(LIBMG)
	$(LINK) $(INVF_MERGE_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

ansi2knr: ansi2knr.o $(LIBMG)
	$(LINK) ansi2knr.o $(LIBS)

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
	rm -f *$o _*.c _*.o *._c *._o core core.*

clean: mostlyclean
	rm -f $(EXEC)
	rm -r bool_parser.c

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

backend.o: ../../sysfuncs.h
backend.o: ../../config.h
backend.o: ../../mg_types.h
backend.o: ../../lib/getopt.h
backend.o: ../../lib/memlib.h
backend.o: ../../lib/messages.h
backend.o: ../../lib/timing.h
backend.o: ../../lib/filestats.h
backend.o: ../../lib/sptree.h
backend.o: ../../lib/netorder.h
backend.o: mg_files.h
backend.o: mg.h
backend.o: invf.h
backend.o: text.h
backend.o: ../../lib/huffman.h
backend.o: lists.h
backend.o: backend.h
backend.o: term_lists.h
backend.o: query_term_list.h
backend.o: stem_search.h
backend.o: invf_get.h
backend.o: text_get.h
backend.o: weights.h
backend.o: locallib.h
backend.o: mg_errors.h
locallib.o: ../../sysfuncs.h
locallib.o: ../../config.h
locallib.o: ../../mg_types.h
locallib.o: ../../lib/getopt.h
locallib.o: ../../lib/memlib.h
locallib.o: ../../lib/bitio_gen.h
locallib.o: ../../lib/huffman.h
locallib.o: ../../lib/filestats.h
locallib.o: ../../lib/netorder.h
locallib.o: locallib.h
locallib.o: text.h
mg_weights_build.o: ../../sysfuncs.h
mg_weights_build.o: ../../config.h
mg_weights_build.o: ../../mg_types.h
mg_weights_build.o: ../../lib/getopt.h
mg_weights_build.o: ../../lib/memlib.h
mg_weights_build.o: ../../lib/messages.h
mg_weights_build.o: ../../lib/local_strings.h
mg_weights_build.o: ../../lib/bitio_gen.h
mg_weights_build.o: ../../lib/bitio_m.h
mg_weights_build.o: ../../lib/bitio_m_stdio.h
mg_weights_build.o: ../../lib/timing.h
mg_weights_build.o: ../../lib/netorder.h
mg_weights_build.o: mg_files.h
mg_weights_build.o: locallib.h
mg_weights_build.o: text.h
mg_weights_build.o: ../../lib/huffman.h
mg_weights_build.o: ../../lib/filestats.h
mg_weights_build.o: invf.h
mg_weights_build.o: words.h
bool_optimiser.o: ../../sysfuncs.h
bool_optimiser.o: ../../config.h
bool_optimiser.o: ../../mg_types.h
bool_optimiser.o: ../../lib/getopt.h
bool_optimiser.o: ../../lib/memlib.h
bool_optimiser.o: bool_tree.h
bool_optimiser.o: term_lists.h
bool_optimiser.o: ../../lib/messages.h
mg.special.o: ../../sysfuncs.h
mg.special.o: ../../config.h
mg.special.o: ../../mg_types.h
mg.special.o: ../../lib/getopt.h
mg.special.o: mg.h
mg.special.o: build.h
mgdictlist.o: ../../sysfuncs.h
mgdictlist.o: ../../config.h
mgdictlist.o: ../../mg_types.h
mgdictlist.o: ../../lib/getopt.h
mgdictlist.o: ../../lib/messages.h
mgdictlist.o: ../../lib/memlib.h
mgdictlist.o: ../../lib/local_strings.h
mgdictlist.o: ../../lib/netorder.h
mgdictlist.o: mg_files.h
mgdictlist.o: text.h
mgdictlist.o: ../../lib/huffman.h
mgdictlist.o: ../../lib/filestats.h
mgdictlist.o: invf.h
mgdictlist.o: locallib.h
mgdictlist.o: words.h
bool_parser.o: ../../sysfuncs.h
bool_parser.o: ../../config.h
bool_parser.o: ../../mg_types.h
bool_parser.o: ../../lib/getopt.h
bool_parser.o: ../../lib/messages.h
bool_parser.o: ../../lib/memlib.h
bool_parser.o: words.h
bool_parser.o: stemmer.h
bool_parser.o: ../../lib/stem.h
bool_parser.o: term_lists.h
bool_parser.o: bool_tree.h
bool_parser.o: backend.h
bool_parser.o: ../../lib/timing.h
bool_parser.o: lists.h
bool_parser.o: query_term_list.h
bool_parser.o: mg.h
bool_parser.o: invf.h
bool_parser.o: text.h
bool_parser.o: ../../lib/huffman.h
bool_parser.o: ../../lib/filestats.h
bool_parser.o: stem_search.h
mg_compression_dict.o: ../../sysfuncs.h
mg_compression_dict.o: ../../config.h
mg_compression_dict.o: ../../mg_types.h
mg_compression_dict.o: ../../lib/getopt.h
mg_compression_dict.o: ../../lib/memlib.h
mg_compression_dict.o: ../../lib/messages.h
mg_compression_dict.o: ../../lib/local_strings.h
mg_compression_dict.o: ../../lib/bitio_gen.h
mg_compression_dict.o: ../../lib/bitio_m.h
mg_compression_dict.o: ../../lib/bitio_m_stdio.h
mg_compression_dict.o: ../../lib/timing.h
mg_compression_dict.o: ../../lib/heap.h
mg_compression_dict.o: ../../lib/netorder.h
mg_compression_dict.o: mg_files.h
mg_compression_dict.o: locallib.h
mg_compression_dict.o: text.h
mg_compression_dict.o: ../../lib/huffman.h
mg_compression_dict.o: ../../lib/filestats.h
mg_compression_dict.o: invf.h
mg_compression_dict.o: words.h
mg_compression_dict.o: mg.h
mgquery.o: ../../sysfuncs.h
mgquery.o: ../../config.h
mgquery.o: ../../mg_types.h
mgquery.o: ../../lib/getopt.h
mgquery.o: ../../lib/rx.h
mgquery.o: ../../lib/messages.h
mgquery.o: ../../lib/timing.h
mgquery.o: ../../lib/memlib.h
mgquery.o: ../../lib/local_strings.h
mgquery.o: ../../lib/filestats.h
mgquery.o: invf.h
mgquery.o: text.h
mgquery.o: ../../lib/huffman.h
mgquery.o: mg.h
mgquery.o: lists.h
mgquery.o: backend.h
mgquery.o: term_lists.h
mgquery.o: query_term_list.h
mgquery.o: environment.h
mgquery.o: globals.h
mgquery.o: read_line.h
mgquery.o: mg_errors.h
mgquery.o: commands.h
mgquery.o: text_get.h
bool_query.o: ../../sysfuncs.h
bool_query.o: ../../config.h
bool_query.o: ../../mg_types.h
bool_query.o: ../../lib/getopt.h
bool_query.o: ../../lib/memlib.h
bool_query.o: ../../lib/messages.h
bool_query.o: ../../lib/filestats.h
bool_query.o: ../../lib/timing.h
bool_query.o: ../../lib/sptree.h
bool_query.o: mg.h
bool_query.o: text.h
bool_query.o: ../../lib/huffman.h
bool_query.o: invf.h
bool_query.o: lists.h
bool_query.o: term_lists.h
bool_query.o: backend.h
bool_query.o: query_term_list.h
bool_query.o: words.h
bool_query.o: stemmer.h
bool_query.o: ../../lib/stem.h
bool_query.o: stem_search.h
bool_query.o: invf_get.h
bool_query.o: text_get.h
bool_query.o: read_line.h
bool_query.o: bool_tree.h
bool_query.o: bool_parser.h
bool_query.o: bool_optimiser.h
bool_query.o: environment.h
mg_errors.o: ../../sysfuncs.h
mg_errors.o: ../../config.h
mg_errors.o: ../../mg_types.h
mg_errors.o: ../../lib/getopt.h
mg_errors.o: ../../lib/memlib.h
mg_errors.o: mg_errors.h
mgstat.o: ../../sysfuncs.h
mgstat.o: ../../config.h
mgstat.o: ../../mg_types.h
mgstat.o: ../../lib/getopt.h
mgstat.o: ../../lib/netorder.h
mgstat.o: mg_files.h
mgstat.o: locallib.h
mgstat.o: text.h
mgstat.o: ../../lib/huffman.h
mgstat.o: ../../lib/filestats.h
mgstat.o: mg.h
mgstat.o: words.h
mgstat.o: invf.h
bool_tester.o: ../../sysfuncs.h
bool_tester.o: ../../config.h
bool_tester.o: ../../mg_types.h
bool_tester.o: ../../lib/getopt.h
bool_tester.o: ../../lib/local_strings.h
bool_tester.o: term_lists.h
bool_tester.o: bool_tree.h
bool_tester.o: bool_parser.h
bool_tester.o: backend.h
bool_tester.o: ../../lib/timing.h
bool_tester.o: lists.h
bool_tester.o: query_term_list.h
bool_tester.o: mg.h
bool_tester.o: invf.h
bool_tester.o: text.h
bool_tester.o: ../../lib/huffman.h
bool_tester.o: ../../lib/filestats.h
bool_tester.o: bool_optimiser.h
mg_fast_comp_dict.o: ../../sysfuncs.h
mg_fast_comp_dict.o: ../../config.h
mg_fast_comp_dict.o: ../../mg_types.h
mg_fast_comp_dict.o: ../../lib/getopt.h
mg_fast_comp_dict.o: ../../lib/filestats.h
mg_fast_comp_dict.o: ../../lib/huffman.h
mg_fast_comp_dict.o: ../../lib/timing.h
mg_fast_comp_dict.o: ../../lib/messages.h
mg_fast_comp_dict.o: ../../lib/memlib.h
mg_fast_comp_dict.o: ../../lib/netorder.h
mg_fast_comp_dict.o: ../../lib/local_strings.h
mg_fast_comp_dict.o: mg.h
mg_fast_comp_dict.o: text.h
mg_fast_comp_dict.o: invf.h
mg_fast_comp_dict.o: lists.h
mg_fast_comp_dict.o: backend.h
mg_fast_comp_dict.o: term_lists.h
mg_fast_comp_dict.o: query_term_list.h
mg_fast_comp_dict.o: mg_files.h
mg_fast_comp_dict.o: locallib.h
mg_fast_comp_dict.o: words.h
mg_decompress_text.o: ../../sysfuncs.h
mg_decompress_text.o: ../../config.h
mg_decompress_text.o: ../../mg_types.h
mg_decompress_text.o: ../../lib/getopt.h
mg_decompress_text.o: ../../lib/filestats.h
mg_decompress_text.o: ../../lib/netorder.h
mg_decompress_text.o: ../../lib/memlib.h
mg_decompress_text.o: ../../lib/messages.h
mg_decompress_text.o: ../../lib/huffman.h
mg_decompress_text.o: ../../lib/bitio_m_mem.h
mg_decompress_text.o: ../../lib/bitio_m.h
mg_decompress_text.o: ../../lib/bitio_stdio.h
mg_decompress_text.o: ../../lib/huffman_stdio.h
mg_decompress_text.o: mg_files.h
mg_decompress_text.o: mg.h
mg_decompress_text.o: backend.h
mg_decompress_text.o: ../../lib/timing.h
mg_decompress_text.o: lists.h
mg_decompress_text.o: term_lists.h
mg_decompress_text.o: query_term_list.h
mg_decompress_text.o: invf.h
mg_decompress_text.o: text.h
mg_decompress_text.o: text_get.h
mg_decompress_text.o: words.h
bool_tree.o: ../../sysfuncs.h
bool_tree.o: ../../config.h
bool_tree.o: ../../mg_types.h
bool_tree.o: ../../lib/getopt.h
bool_tree.o: ../../lib/memlib.h
bool_tree.o: ../../lib/messages.h
bool_tree.o: bool_tree.h
bool_tree.o: term_lists.h
mg_files.o: ../../sysfuncs.h
mg_files.o: ../../config.h
mg_files.o: ../../mg_types.h
mg_files.o: ../../lib/getopt.h
mg_files.o: ../../lib/memlib.h
mg_files.o: ../../lib/messages.h
mg_files.o: ../../lib/netorder.h
mg_files.o: mg_files.h
query.docnums.o: ../../sysfuncs.h
query.docnums.o: ../../config.h
query.docnums.o: ../../mg_types.h
query.docnums.o: ../../lib/getopt.h
query.docnums.o: ../../lib/filestats.h
query.docnums.o: ../../lib/timing.h
query.docnums.o: mg.h
query.docnums.o: invf.h
query.docnums.o: text.h
query.docnums.o: ../../lib/huffman.h
query.docnums.o: lists.h
query.docnums.o: backend.h
query.docnums.o: term_lists.h
query.docnums.o: query_term_list.h
commands.o: ../../sysfuncs.h
commands.o: ../../config.h
commands.o: ../../mg_types.h
commands.o: ../../lib/getopt.h
commands.o: ../../lib/memlib.h
commands.o: ../../lib/messages.h
commands.o: environment.h
commands.o: mg.h
commands.o: locallib.h
commands.o: text.h
commands.o: ../../lib/huffman.h
commands.o: ../../lib/filestats.h
commands.o: backend.h
commands.o: ../../lib/timing.h
commands.o: lists.h
commands.o: term_lists.h
commands.o: query_term_list.h
commands.o: invf.h
commands.o: help.mg.h
commands.o: warranty.h
commands.o: conditions.h
mg_hilite_words.o: ../../sysfuncs.h
mg_hilite_words.o: ../../config.h
mg_hilite_words.o: ../../mg_types.h
mg_hilite_words.o: ../../lib/getopt.h
mg_hilite_words.o: ../../lib/messages.h
mg_hilite_words.o: ../../lib/local_strings.h
mg_hilite_words.o: stemmer.h
mg_hilite_words.o: ../../lib/stem.h
mg_hilite_words.o: words.h
mg_hilite_words.o: hash.h
mg_hilite_words.o: locallib.h
mg_hilite_words.o: text.h
mg_hilite_words.o: ../../lib/huffman.h
mg_hilite_words.o: ../../lib/filestats.h
query.ranked.o: ../../sysfuncs.h
query.ranked.o: ../../config.h
query.ranked.o: ../../mg_types.h
query.ranked.o: ../../lib/getopt.h
query.ranked.o: ../../lib/memlib.h
query.ranked.o: ../../lib/filestats.h
query.ranked.o: ../../lib/messages.h
query.ranked.o: ../../lib/timing.h
query.ranked.o: ../../lib/sptree.h
query.ranked.o: mg.h
query.ranked.o: invf.h
query.ranked.o: text.h
query.ranked.o: ../../lib/huffman.h
query.ranked.o: lists.h
query.ranked.o: backend.h
query.ranked.o: term_lists.h
query.ranked.o: query_term_list.h
query.ranked.o: stem_search.h
query.ranked.o: weights.h
query.ranked.o: text_get.h
query.ranked.o: invf_get.h
query.ranked.o: words.h
query.ranked.o: stemmer.h
query.ranked.o: ../../lib/stem.h
query.ranked.o: locallib.h
query.ranked.o: environment.h
query.ranked.o: ../../lib/local_strings.h
comp_dict.o: ../../sysfuncs.h
comp_dict.o: ../../config.h
comp_dict.o: ../../mg_types.h
comp_dict.o: ../../lib/getopt.h
comp_dict.o: ../../lib/huffman.h
comp_dict.o: ../../lib/filestats.h
comp_dict.o: ../../lib/local_strings.h
comp_dict.o: ../../lib/memlib.h
comp_dict.o: ../../lib/messages.h
comp_dict.o: mg.h
comp_dict.o: hash.h
comp_dict.o: text.h
comp_dict.o: comp_dict.h
comp_dict.o: locallib.h
comp_dict.o: mg_files.h
mg_invf_dict.o: ../../sysfuncs.h
mg_invf_dict.o: ../../config.h
mg_invf_dict.o: ../../mg_types.h
mg_invf_dict.o: ../../lib/getopt.h
mg_invf_dict.o: ../../lib/memlib.h
mg_invf_dict.o: ../../lib/messages.h
mg_invf_dict.o: ../../lib/netorder.h
mg_invf_dict.o: mg_files.h
mg_invf_dict.o: invf.h
mg_invf_dict.o: locallib.h
mg_invf_dict.o: text.h
mg_invf_dict.o: ../../lib/huffman.h
mg_invf_dict.o: ../../lib/filestats.h
mg_invf_dict.o: words.h
mg_invf_dict.o: mg.h
read_line.o: ../../sysfuncs.h
read_line.o: ../../config.h
read_line.o: ../../mg_types.h
read_line.o: ../../lib/getopt.h
read_line.o: ../../lib/memlib.h
read_line.o: globals.h
read_line.o: environment.h
read_line.o: read_line.h
environment.o: ../../sysfuncs.h
environment.o: ../../config.h
environment.o: ../../mg_types.h
environment.o: ../../lib/getopt.h
environment.o: ../../lib/memlib.h
environment.o: locallib.h
environment.o: text.h
environment.o: ../../lib/huffman.h
environment.o: ../../lib/filestats.h
environment.o: ../../lib/local_strings.h
environment.o: ../../lib/messages.h
environment.o: environment.h
mg_invf_dump.o: ../../sysfuncs.h
mg_invf_dump.o: ../../config.h
mg_invf_dump.o: ../../mg_types.h
mg_invf_dump.o: ../../lib/getopt.h
mg_invf_dump.o: ../../lib/messages.h
mg_invf_dump.o: ../../lib/timing.h
mg_invf_dump.o: ../../lib/bitio_m.h
mg_invf_dump.o: ../../lib/bitio_m_stdio.h
mg_invf_dump.o: ../../lib/bitio_gen.h
mg_invf_dump.o: ../../lib/netorder.h
mg_invf_dump.o: mg_files.h
mg_invf_dump.o: locallib.h
mg_invf_dump.o: text.h
mg_invf_dump.o: ../../lib/huffman.h
mg_invf_dump.o: ../../lib/filestats.h
mg_invf_dump.o: words.h
mg_invf_dump.o: invf.h
stem_search.o: ../../sysfuncs.h
stem_search.o: ../../config.h
stem_search.o: ../../mg_types.h
stem_search.o: ../../lib/getopt.h
stem_search.o: ../../lib/memlib.h
stem_search.o: ../../lib/messages.h
stem_search.o: ../../lib/filestats.h
stem_search.o: ../../lib/timing.h
stem_search.o: ../../lib/local_strings.h
stem_search.o: ../../lib/netorder.h
stem_search.o: mg.h
stem_search.o: invf.h
stem_search.o: text.h
stem_search.o: ../../lib/huffman.h
stem_search.o: lists.h
stem_search.o: backend.h
stem_search.o: term_lists.h
stem_search.o: query_term_list.h
stem_search.o: words.h
stem_search.o: locallib.h
stem_search.o: stem_search.h
stem_search.o: mg_errors.h
stem_search.o: stemmer.h
stem_search.o: ../../lib/stem.h
mg_invf_merge.o: ../../sysfuncs.h
mg_invf_merge.o: ../../config.h
mg_invf_merge.o: ../../mg_types.h
mg_invf_merge.o: ../../lib/getopt.h
mg_invf_merge.o: ../../lib/memlib.h
mg_invf_merge.o: locallib.h
mg_invf_merge.o: text.h
mg_invf_merge.o: ../../lib/huffman.h
mg_invf_merge.o: ../../lib/filestats.h
mg_invf_merge.o: ../../lib/local_strings.h
mg_invf_merge.o: ../../lib/messages.h
mg_invf_merge.o: ../../lib/timing.h
mg_invf_merge.o: ../../lib/netorder.h
mg_invf_merge.o: ../../lib/bitio_m.h
mg_invf_merge.o: ../../lib/bitio_gen.h
mg_invf_merge.o: ../../lib/bitio_stdio.h
mg_invf_merge.o: mg.h
mg_invf_merge.o: mg_merge.h
mg_invf_merge.o: mg_files.h
mg_invf_merge.o: invf.h
mg_invf_merge.o: words.h
stemmer.o: ../../sysfuncs.h
stemmer.o: ../../config.h
stemmer.o: ../../mg_types.h
stemmer.o: ../../lib/getopt.h
stemmer.o: stemmer.h
stemmer.o: ../../lib/stem.h
mg_stem_idx.o: ../../sysfuncs.h
mg_stem_idx.o: ../../config.h
mg_stem_idx.o: ../../mg_types.h
mg_stem_idx.o: ../../lib/getopt.h
mg_stem_idx.o: ../../lib/memlib.h
mg_stem_idx.o: ../../lib/messages.h
mg_stem_idx.o: ../../lib/filestats.h
mg_stem_idx.o: ../../lib/netorder.h
mg_stem_idx.o: mg_files.h
mg_stem_idx.o: invf.h
mg_stem_idx.o: mg.h
mg_stem_idx.o: locallib.h
mg_stem_idx.o: text.h
mg_stem_idx.o: ../../lib/huffman.h
mg_stem_idx.o: backend.h
mg_stem_idx.o: ../../lib/timing.h
mg_stem_idx.o: lists.h
mg_stem_idx.o: term_lists.h
mg_stem_idx.o: query_term_list.h
mg_stem_idx.o: words.h
mg_stem_idx.o: stemmer.h
mg_stem_idx.o: ../../lib/stem.h
mg_stem_idx.o: hash.h
mg_stem_idx.o: ../../lib/local_strings.h
mg_invf_rebuild.o: ../../sysfuncs.h
mg_invf_rebuild.o: ../../config.h
mg_invf_rebuild.o: ../../mg_types.h
mg_invf_rebuild.o: ../../lib/getopt.h
mg_invf_rebuild.o: ../../lib/memlib.h
mg_invf_rebuild.o: ../../lib/messages.h
mg_invf_rebuild.o: ../../lib/timing.h
mg_invf_rebuild.o: ../../lib/bitio_m.h
mg_invf_rebuild.o: ../../lib/bitio_m_stdio.h
mg_invf_rebuild.o: ../../lib/bitio_gen.h
mg_invf_rebuild.o: ../../lib/netorder.h
mg_invf_rebuild.o: mg_files.h
mg_invf_rebuild.o: invf.h
mg_invf_rebuild.o: locallib.h
mg_invf_rebuild.o: text.h
mg_invf_rebuild.o: ../../lib/huffman.h
mg_invf_rebuild.o: ../../lib/filestats.h
mg_invf_rebuild.o: words.h
mg_invf_rebuild.o: mg.h
term_lists.o: ../../sysfuncs.h
term_lists.o: ../../config.h
term_lists.o: ../../mg_types.h
term_lists.o: ../../lib/getopt.h
term_lists.o: ../../lib/memlib.h
term_lists.o: ../../lib/local_strings.h
term_lists.o: term_lists.h
term_lists.o: ../../lib/messages.h
term_lists.o: stemmer.h
term_lists.o: ../../lib/stem.h
mgstemidxlist.o: ../../sysfuncs.h
mgstemidxlist.o: ../../config.h
mgstemidxlist.o: ../../mg_types.h
mgstemidxlist.o: ../../lib/getopt.h
mgstemidxlist.o: ../../lib/messages.h
mgstemidxlist.o: ../../lib/memlib.h
mgstemidxlist.o: ../../lib/local_strings.h
mgstemidxlist.o: ../../lib/netorder.h
mgstemidxlist.o: mg_files.h
mgstemidxlist.o: mg.h
mgstemidxlist.o: invf.h
mgstemidxlist.o: words.h
mgstemidxlist.o: backend.h
mgstemidxlist.o: ../../lib/timing.h
mgstemidxlist.o: lists.h
mgstemidxlist.o: term_lists.h
mgstemidxlist.o: query_term_list.h
mgstemidxlist.o: text.h
mgstemidxlist.o: ../../lib/huffman.h
mgstemidxlist.o: ../../lib/filestats.h
invf_get.o: ../../sysfuncs.h
invf_get.o: ../../config.h
invf_get.o: ../../mg_types.h
invf_get.o: ../../lib/getopt.h
invf_get.o: ../../lib/memlib.h
invf_get.o: ../../lib/filestats.h
invf_get.o: ../../lib/timing.h
invf_get.o: ../../lib/bitio_gen.h
invf_get.o: ../../lib/bitio_m_mem.h
invf_get.o: ../../lib/bitio_m.h
invf_get.o: ../../lib/sptree.h
invf_get.o: ../../lib/messages.h
invf_get.o: ../../lib/netorder.h
invf_get.o: mg.h
invf_get.o: invf.h
invf_get.o: text.h
invf_get.o: ../../lib/huffman.h
invf_get.o: lists.h
invf_get.o: backend.h
invf_get.o: term_lists.h
invf_get.o: query_term_list.h
invf_get.o: invf_get.h
invf_get.o: mg_errors.h
mg_passes.o: ../../sysfuncs.h
mg_passes.o: ../../config.h
mg_passes.o: ../../mg_types.h
mg_passes.o: ../../lib/getopt.h
mg_passes.o: ../../lib/memlib.h
mg_passes.o: ../../lib/messages.h
mg_passes.o: ../../lib/timing.h
mg_passes.o: mg_files.h
mg_passes.o: mg.h
mg_passes.o: build.h
mg_passes.o: text.h
mg_passes.o: ../../lib/huffman.h
mg_passes.o: ../../lib/filestats.h
mg_passes.o: stemmer.h
mg_passes.o: ../../lib/stem.h
text.pass1.o: ../../sysfuncs.h
text.pass1.o: ../../config.h
text.pass1.o: ../../mg_types.h
text.pass1.o: ../../lib/getopt.h
text.pass1.o: ../../lib/memlib.h
text.pass1.o: ../../lib/messages.h
text.pass1.o: ../../lib/huffman.h
text.pass1.o: ../../lib/filestats.h
text.pass1.o: ../../lib/netorder.h
text.pass1.o: mg_files.h
text.pass1.o: mg.h
text.pass1.o: build.h
text.pass1.o: locallib.h
text.pass1.o: text.h
text.pass1.o: words.h
text.pass1.o: hash.h
text.pass1.o: ../../lib/local_strings.h
ivf.pass1.o: ../../sysfuncs.h
ivf.pass1.o: ../../config.h
ivf.pass1.o: ../../mg_types.h
ivf.pass1.o: ../../lib/getopt.h
ivf.pass1.o: ../../lib/memlib.h
ivf.pass1.o: ../../lib/messages.h
ivf.pass1.o: ../../lib/bitio_m.h
ivf.pass1.o: ../../lib/bitio_m_stdio.h
ivf.pass1.o: ../../lib/bitio_gen.h
ivf.pass1.o: ../../lib/local_strings.h
ivf.pass1.o: ../../lib/netorder.h
ivf.pass1.o: mg_files.h
ivf.pass1.o: invf.h
ivf.pass1.o: mg.h
ivf.pass1.o: build.h
ivf.pass1.o: locallib.h
ivf.pass1.o: text.h
ivf.pass1.o: ../../lib/huffman.h
ivf.pass1.o: ../../lib/filestats.h
ivf.pass1.o: words.h
ivf.pass1.o: stemmer.h
ivf.pass1.o: ../../lib/stem.h
ivf.pass1.o: hash.h
mg_perf_hash_build.o: ../../sysfuncs.h
mg_perf_hash_build.o: ../../config.h
mg_perf_hash_build.o: ../../mg_types.h
mg_perf_hash_build.o: ../../lib/getopt.h
mg_perf_hash_build.o: ../../lib/memlib.h
mg_perf_hash_build.o: ../../lib/messages.h
mg_perf_hash_build.o: ../../lib/timing.h
mg_perf_hash_build.o: ../../lib/local_strings.h
mg_perf_hash_build.o: ../../lib/perf_hash.h
mg_perf_hash_build.o: ../../lib/netorder.h
mg_perf_hash_build.o: mg_files.h
mg_perf_hash_build.o: invf.h
mg_perf_hash_build.o: locallib.h
mg_perf_hash_build.o: text.h
mg_perf_hash_build.o: ../../lib/huffman.h
mg_perf_hash_build.o: ../../lib/filestats.h
mg_perf_hash_build.o: words.h
mg_perf_hash_build.o: mg.h
text.pass2.o: ../../sysfuncs.h
text.pass2.o: ../../config.h
text.pass2.o: ../../mg_types.h
text.pass2.o: ../../lib/getopt.h
text.pass2.o: ../../lib/memlib.h
text.pass2.o: ../../lib/messages.h
text.pass2.o: ../../lib/local_strings.h
text.pass2.o: ../../lib/bitio_m_mem.h
text.pass2.o: ../../lib/bitio_m.h
text.pass2.o: ../../lib/huffman.h
text.pass2.o: ../../lib/filestats.h
text.pass2.o: ../../lib/bitio_stdio.h
text.pass2.o: ../../lib/huffman_stdio.h
text.pass2.o: ../../lib/netorder.h
text.pass2.o: mg.h
text.pass2.o: mg_files.h
text.pass2.o: build.h
text.pass2.o: words.h
text.pass2.o: text.h
text.pass2.o: hash.h
text.pass2.o: locallib.h
text.pass2.o: comp_dict.h
ivf.pass2.o: ../../lib/local_strings.h
ivf.pass2.o: ../../sysfuncs.h
ivf.pass2.o: ../../config.h
ivf.pass2.o: ../../mg_types.h
ivf.pass2.o: ../../lib/getopt.h
ivf.pass2.o: ../../lib/memlib.h
ivf.pass2.o: ../../lib/messages.h
ivf.pass2.o: stemmer.h
ivf.pass2.o: ../../lib/stem.h
ivf.pass2.o: ../../lib/perf_hash.h
ivf.pass2.o: ../../lib/bitio_m.h
ivf.pass2.o: ../../lib/bitio_m_mems.h
ivf.pass2.o: ../../lib/bitio_gen.h
ivf.pass2.o: ../../lib/bitio_random.h
ivf.pass2.o: ../../lib/bitio_stdio.h
ivf.pass2.o: ../../lib/netorder.h
ivf.pass2.o: mg_files.h
ivf.pass2.o: invf.h
ivf.pass2.o: locallib.h
ivf.pass2.o: text.h
ivf.pass2.o: ../../lib/huffman.h
ivf.pass2.o: ../../lib/filestats.h
ivf.pass2.o: mg.h
ivf.pass2.o: build.h
ivf.pass2.o: words.h
ivf.pass2.o: hash.h
mg_text_estimate.o: ../../sysfuncs.h
mg_text_estimate.o: ../../config.h
mg_text_estimate.o: ../../mg_types.h
mg_text_estimate.o: ../../lib/getopt.h
mg_text_estimate.o: ../../lib/memlib.h
mg_text_estimate.o: ../../lib/messages.h
mg_text_estimate.o: ../../lib/local_strings.h
mg_text_estimate.o: ../../lib/bitio_gen.h
mg_text_estimate.o: ../../lib/bitio_m.h
mg_text_estimate.o: ../../lib/bitio_m_stdio.h
mg_text_estimate.o: ../../lib/timing.h
mg_text_estimate.o: ../../lib/heap.h
mg_text_estimate.o: ../../lib/netorder.h
mg_text_estimate.o: mg_files.h
mg_text_estimate.o: locallib.h
mg_text_estimate.o: text.h
mg_text_estimate.o: ../../lib/huffman.h
mg_text_estimate.o: ../../lib/filestats.h
mg_text_estimate.o: invf.h
mg_text_estimate.o: words.h
mg_text_estimate.o: mg.h
mg_text_estimate.o: comp_dict.h
mg_text_estimate.o: hash.h
text_get.o: ../../sysfuncs.h
text_get.o: ../../config.h
text_get.o: ../../mg_types.h
text_get.o: ../../lib/getopt.h
text_get.o: ../../lib/memlib.h
text_get.o: ../../lib/filestats.h
text_get.o: ../../lib/timing.h
text_get.o: ../../lib/messages.h
text_get.o: ../../lib/netorder.h
text_get.o: ../../lib/huffman.h
text_get.o: ../../lib/bitio_m_mem.h
text_get.o: ../../lib/bitio_m.h
text_get.o: ../../lib/bitio_stdio.h
text_get.o: ../../lib/huffman_stdio.h
text_get.o: mg.h
text_get.o: invf.h
text_get.o: text.h
text_get.o: lists.h
text_get.o: backend.h
text_get.o: term_lists.h
text_get.o: query_term_list.h
text_get.o: text_get.h
text_get.o: locallib.h
text_get.o: words.h
text_get.o: mg_errors.h
text_get.o: ../../lib/local_strings.h
lists.o: ../../sysfuncs.h
lists.o: ../../config.h
lists.o: ../../mg_types.h
lists.o: ../../lib/getopt.h
lists.o: ../../lib/memlib.h
lists.o: lists.h
lists.o: ../../lib/local_strings.h
lists.o: locallib.h
lists.o: text.h
lists.o: ../../lib/huffman.h
lists.o: ../../lib/filestats.h
mg_text_merge.o: ../../sysfuncs.h
mg_text_merge.o: ../../config.h
mg_text_merge.o: ../../mg_types.h
mg_text_merge.o: ../../lib/getopt.h
mg_text_merge.o: ../../lib/messages.h
mg_text_merge.o: ../../lib/timing.h
mg_text_merge.o: ../../lib/netorder.h
mg_text_merge.o: locallib.h
mg_text_merge.o: text.h
mg_text_merge.o: ../../lib/huffman.h
mg_text_merge.o: ../../lib/filestats.h
mg_text_merge.o: mg.h
mg_text_merge.o: mg_merge.h
mg_text_merge.o: mg_files.h
weights.o: ../../sysfuncs.h
weights.o: ../../config.h
weights.o: ../../mg_types.h
weights.o: ../../lib/getopt.h
weights.o: ../../lib/filestats.h
weights.o: ../../lib/memlib.h
weights.o: ../../lib/messages.h
weights.o: ../../lib/timing.h
weights.o: ../../lib/netorder.h
weights.o: mg.h
weights.o: invf.h
weights.o: text.h
weights.o: ../../lib/huffman.h
weights.o: lists.h
weights.o: backend.h
weights.o: term_lists.h
weights.o: query_term_list.h
weights.o: weights.h
weights.o: locallib.h
weights.o: mg_errors.h
query_term_list.o: ../../sysfuncs.h
query_term_list.o: ../../config.h
query_term_list.o: ../../mg_types.h
query_term_list.o: ../../lib/getopt.h
query_term_list.o: ../../lib/memlib.h
query_term_list.o: ../../lib/local_strings.h
query_term_list.o: query_term_list.h
query_term_list.o: term_lists.h
query_term_list.o: ../../lib/messages.h
