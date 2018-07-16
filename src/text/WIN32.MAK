#PACKAGE = mg
#VERSION = 1.2

#SHELL = /bin/sh
#exec_prefix = ${prefix}
#prefix = /image/httpd/mg-1.2
#srcdir = .
#subdir = src/text
#top_srcdir = ../..
#transform = s,x,x,

CC = cl
#CC = cl /Z7  this gives symbol and line number debugging (set /DEBUG on link)
#CFLAGS = -g -O
CPPFLAGS =
DEFS = -DHAVE_CONFIG_H -D__WIN32__ -D_LITTLE_ENDIAN
#INSTALL = /usr/local/gnu/bin/install -c
#INSTALL_DATA = ${INSTALL} -m 644
#INSTALL_PROGRAM = ${INSTALL}
LDFLAGS =
LIBS = ..\..\lib\libmg.lib
#TOUCH = echo timestamp >
#AWK = gawk

#bindir = $(exec_prefix)/bin
#datadir = $(prefix)/share
#libexecdir = $(exec_prefix)/libexec
#localedir = $(datadir)/locale
#mandir = $(prefix)/man/man1

COMPILE = $(CC) -c $(DEFS) -I. -I..\.. -I..\..\lib
LINK = $(CC) $(LDFLAGS)

.SUFFIXES:
.SUFFIXES: .c .obj .cpp
.c.obj:
	$(COMPILE) $<
.cpp.obj:
	$(COMPILE) $<

o = .obj
e = .exe

#############################################################################
#
# Uncomment (and possibly edit) the following lines if you wish
# to use the GNU readline library for the input in mgquery
#
#READLINE = -DGNU_READLINE -I/opt/gnu/include/readline -L/opt/gnu/lib
#READLINE_LIBS = -lreadline -ltermcap
#
#
#############################################################################

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
  query_term_list.c       mgpass.c                builder.cpp

HEADERS = \
  backend.h          conditions.h       locallib.h         term_lists.h \
  bool_optimiser.h   environment.h      mg.h               text.h       \
  bool_parser.h      globals.h          mg_errors.h        text_get.h   \
  bool_query.h       hash.h             mg_files.h         warranty.h   \
  bool_tree.h        help.mg.h          mg_merge.h         weights.h    \
  build.h            invf.h             read_line.h        words.h      \
  commands.h         invf_get.h         stem_search.h                   \
  comp_dict.h        lists.h            stemmer.h          query_term_list.h


#MISC          = Makefile.in help.mg.src bool_parser.y warranty.src conditions.src

ALLFILES      = $(SOURCES) $(HEADERS) $(MISC) $(MAN)

DISTFILES     = $(ALLFILES)

EXEC          = mgquery$e mg_weights_build$e mgstat$e \
		mg_invf_dump$e     mg_invf_dict$e \
		mg_invf_rebuild$e     mgdictlist$e \
		mg_passes$e     mg_perf_hash_build$e \
		mg_compression_dict$e     mg_text_estimate$e \
		mg_fast_comp_dict$e \
		mg_hilite_words$e     mg_invf_merge$e \
		mg_text_merge$e     bool_tester$e  mgpass$e \
		mg_stem_idx$e  mgstemidxlist$e  mg_decompress_text$e \
		builder$e

MAN           = mgquery.1 mg_weights_build.1 mgstat.1 mg_invf_dump.1 mg_invf_dict.1 \
		mg_invf_rebuild.1 mgdictlist.1 mg_passes.1 mg_perf_hash_build.1 \
		mg_compression_dict.1 mg_text_estimate.1 mg_fast_comp_dict.1 \
		mg_hilite_words.1 mg_invf_merge.1 mg_text_merge.1 \
		mg_stem_idx.1 mgstemidxlist.1 mg_decompress_text.1

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


all:    $(EXEC)


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#help.mg.h : help.mg.src
#        sed -e 's/\\/\\\\/g' -e 's/\"/\\\"/g' help.mg.src | $(AWK) '/^#/ { next; };{ if (x == 0) printf("static char *help_str[] = {\n\"%s\\n\"", $$0); else printf(",\n\"%s\\n\"", $$0); x = 1; }; END { printf("};\n"); }' - >help.mg.h

#warranty.h : warranty.src
#        sed  -e 's/\\/\\\\/g' -e 's/\"/\\\"/g' warranty.src | $(AWK) '/^#/ { next; };{ if (x == 0) printf("static char *warranty_str[] = {\n\"%s\\n\"", $$0); else printf(",\n\"%s\\n\"", $$0); x = 1; }; END { printf("};\n"); }' - >warranty.h

#conditions.h : conditions.src
#        sed -e 's/\\/\\\\/g' -e 's/\"/\\\"/g' conditions.src | $(AWK) '/^#/ { next; };{ if (x == 0) printf("static char *cond_str[] = {\n\"%s\\n\"", $$0); else printf(",\n\"%s\\n\"", $$0); x = 1; }; END { printf("};\n"); }' - >conditions.h


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

HILITE_OBJS = mg_hilite_words$o stemmer$o locallib$o

mg_hilite_words$e: $(HILITE_OBJS)
	$(LINK) $(HILITE_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#bool_parser.c: bool_parser.y
#        $(YACC) $?
#        mv y.tab.c $@
#
BOOL_OBJS = bool_tree$o bool_parser$o bool_optimiser$o bool_tester$o \
	    term_lists$o stemmer$o stem_search$o mg_errors$o query_term_list$o

bool_tester$e: $(BOOL_OBJS)
	$(LINK) $(BOOL_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

QUERY_OBJS = \
	mgquery$o locallib$o lists$o \
	query.ranked$o query.docnums$o stem_search$o \
	environment$o commands$o weights$o text_get$o stemmer$o \
	read_line$o mg_errors$o backend$o invf_get$o \
	term_lists$o bool_tree$o bool_parser$o bool_optimiser$o bool_query$o \
	query_term_list$o

mgquery$e: $(QUERY_OBJS)
	$(LINK) $(READLINE) $(QUERY_OBJS) $(READLINE_LIBS) $(LIBS)

commands$o : help.mg.h warranty.h conditions.h

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

PASSES_OBJS = \
	mg_passes$o text.pass1$o comp_dict$o stemmer$o \
	text.pass2$o locallib$o \
	ivf.pass1$o ivf.pass2$o mg.special$o mg_files$o

mg_passes$e: $(PASSES_OBJS)
	$(LINK) $(PASSES_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

PASS_OBJS = \
	mgpass$o text.pass1$o comp_dict$o stemmer$o \
	text.pass2$o locallib$o \
	ivf.pass1$o ivf.pass2$o mg.special$o mg_files$o

#mgpass$o:  mgpass.cpp
#	$(COMPILE) mgpass.cpp

mgpass$e: $(PASS_OBJS)
	$(LINK) $(PASS_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib

BUILDER_OBJS = \
	builder$o

builder$e: $(BUILDER_OBJS)
	$(LINK) $(BUILDER_OBJS) $(LIBS) $(LINK32_FLAGS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

WEIGHTS_BUILD_OBJS = mg_weights_build$o mg_files$o

mg_weights_build$e: $(WEIGHTS_BUILD_OBJS)
	$(LINK) $(WEIGHTS_BUILD_OBJS) $(LIBS)

COMP_DICT_OBJS = mg_compression_dict$o mg_files$o locallib$o

mg_compression_dict$e: $(COMP_DICT_OBJS)
	$(LINK) $(COMP_DICT_OBJS) $(LIBS)

FAST_COMP_OBJS = mg_fast_comp_dict$o mg_files$o locallib$o

mg_fast_comp_dict$e: $(FAST_COMP_OBJS)
	$(LINK) $(FAST_COMP_OBJS) $(LIBS)

TEXT_EST_OBJS = mg_text_estimate$o locallib$o comp_dict$o mg_files$o

mg_text_estimate$e: $(TEXT_EST_OBJS)
	$(LINK) $(TEXT_EST_OBJS) $(LIBS)

STAT_OBJS = mgstat$o mg_files$o locallib$o

mgstat$e: $(STAT_OBJS)
	$(LINK) $(STAT_OBJS) $(LIBS)

INVF_DUMP_OBJS = mg_invf_dump$o locallib$o mg_files$o

mg_invf_dump$e: $(INVF_DUMP_OBJS)
	$(LINK) $(INVF_DUMP_OBJS) $(LIBS)

INVF_DICT_OBJS = mg_invf_dict$o mg_files$o locallib$o

mg_invf_dict$e: $(INVF_DICT_OBJS)
	$(LINK) $(INVF_DICT_OBJS) $(LIBS)

INVF_REB_OBJS = mg_invf_rebuild$o locallib$o

mg_invf_rebuild$e: $(INVF_REB_OBJS)
	$(LINK) $(INVF_REB_OBJS) $(LIBS)

DICTLIST_OBJS = mgdictlist$o locallib$o

mgdictlist$e: $(DICTLIST_OBJS)
	$(LINK) $(DICTLIST_OBJS) $(LIBS)

PERF_HASH_OBJS = mg_perf_hash_build$o mg_files$o

mg_perf_hash_build$e: $(PERF_HASH_OBJS)
	$(LINK) $(PERF_HASH_OBJS) $(LIBS)

STEM_IDX_OBJS = \
                mg_stem_idx$o mg_files$o stemmer$o locallib$o \
                mg_errors$o term_lists$o

mg_stem_idx$e: $(STEM_IDX_OBJS)
	$(LINK) $(STEM_IDX_OBJS) $(LIBS)

STEM_IDX_LIST_OBJS = mgstemidxlist$o mg_files$o

mgstemidxlist$e: $(STEM_IDX_LIST_OBJS)
	$(LINK) $(STEM_IDX_LIST_OBJS) $(LIBS)

DECOMPRESS_TEXT_OBJS = \
                mg_decompress_text$o mg_files$o mg_errors$o \
                text_get$o locallib$o backend$o stem_search$o \
                term_lists$o stemmer$o weights$o invf_get$o lists$o

mg_decompress_text$e: $(DECOMPRESS_TEXT_OBJS)
	$(LINK) $(DECOMPRESS_TEXT_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

TEXT_MERGE_OBJS = mg_text_merge$o mg_files$o locallib$o

mg_text_merge$e: $(TEXT_MERGE_OBJS)
	$(LINK) $(TEXT_MERGE_OBJS) $(LIBS)

INVF_MERGE_OBJS = mg_invf_merge$o mg_files$o locallib$o

mg_invf_merge$e: $(INVF_MERGE_OBJS)
	$(LINK) $(INVF_MERGE_OBJS) $(LIBS)

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

#ansi2knr: ansi2knr.o
#        $(LINK) ansi2knr.o $(LIBS)
#
install: execinstall maninstall
uninstall: execuninstall manuninstall

#execinstall: $(EXEC)
#        $(top_srcdir)/mkinstalldirs $(bindir); \
#        for name in $(EXEC); do \
#          $(INSTALL_PROGRAM) $$name \
#          $(bindir)/`echo $$name | sed '$(transform)'`; \
#        done

#execuninstall:
#        for name in $(EXEC); do \
#         rm -f $(bindir)/`echo $$name | sed '$(transform)'`; \
#        done

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
#        for file in $(DISTFILES); do \
#          ln $(srcdir)/$$file $(distdir) 2> /dev/null \
#          || cp -p $(srcdir)/$$file $(distdir); \
#        done
 
#Makefile: Makefile.in ../../config.status
#        cd ../.. && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= ./config.status
 
# Tell versions [3.59,3.63) of GNU make not to export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
#.NOEXPORT:
