CC = gcc
CFLAGS = -g -O2 
CPPFLAGS = 
INCLUDES = -I..
RM	= rm -f

DISTFILES = Makefile q1 q2 q3 r1 r2 r3 mgrc1 mgrc2 mgrc3 testmg \
            alice.sum simple_sum.c alice.len

COMPILEFLAGS = $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
COMPILE = $(CC) -c $(COMPILEFLAGS)

.SUFFIXES:
.SUFFIXES: .c .o
.c.o:
	$(COMPILE) $<

all: simple_sum

check:

dist:
	ln $(DISTFILES) $(distdir)

simple_sum: simple_sum.c
	$(CC) $(COMPILEFLAGS) -o $@ $?

clean:
	-$(RM) core a.out *~ \#*
	-$(RM) simple_sum *.cmp *.log *.diff
	-$(RM) -r mgdata

mostlyclean:	clean

distclean:	mostlyclean

maintainer-clean:	distclean

depend:
	cp Makefile Makefile.tmp
	makedepend -f Makefile.tmp -w 0 -- $(COMPILEFLAGS) -- $(SOURCES)
	grep -v '[.]o: [/]' Makefile.tmp >Makefile
	rm -f Makefile.tmp

# DO NOT DELETE THIS LINE -- make depend depends on it.
