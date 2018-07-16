DISTFILES = Makefile mgmerge.README mg_invf_dump.changes \
  README.man mg.1 mgintro.1 mgintro++.1

SHELL		= /bin/sh

all:	dist

dist:
	ln $(DISTFILES) $(distdir)

mostlyclean:
	rm -f *$o _*.c _*.o *._c *._o core core.*
 
clean: mostlyclean
 
distclean: clean
	rm -f Makefile
 
maintainer-clean: distclean
	@echo "This command is intended only for maintainers to use;"
	@echo "rebuilding the deleted files may require special tools."
