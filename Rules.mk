# We are trying the monolithic make concept, and using Emile van Bergen's
# ideas.
#
# Note that the Delphi compiler does its own determination of what does
# and doesn't need compiling
#
.SUFFIXES:
.SUFFIXES: .c .o .cc

all: targets
#
# subdirectories, no particular order
dir := XFitLibrary
include $(dir)/Rules.mk

%.o: %.c
	$(COMP)

%.o: %.cxx
	$(C++COMP)

# stuff for the top-level directory
TGT_BIN := $(TGT_BIN) xfit

CLEAN := $(CLEAN) *.dcu xfit
#
# The variables TGT_*, CLEAN and CMD_INST* may be added to by the Makefile
# fragments in the various subdirectories.

.PHONY:		targets
targets:	$(TGT_BIN)

.PHONY:		clean
clean:
		rm -f $(CLEAN)

.PHONY:		install
install:	targets 
		$(INST) -d $(DIR_BIN) $(DIR_LIB)
		$(INST) -m 750 $(TGT_BIN) $(DIR_BIN)
		$(INST) -m 750 $(TGT_LIB) $(DIR_LIB)
		$(INST) -m 750 /usr/local/kylix2/bin/libqtintf-6.5.0-qt2.3.so $(DIR_LIB)
		(cd $(DIR_LIB);ln -s libqtintf-6.5.0-qt2.3.so libqtintf-6.5-qt2.3.so)
		$(INST) -d $(DESTDIR)/usr/share/doc/xfit
		$(INST) -d $(DESTDIR)/usr/share/man/man1
		$(INST) -m 755 debian/xfit.1 $(DESTDIR)/usr/share/man/man1
		cp help/* $(DESTDIR)/usr/share/doc/xfit


# Prevent make from removing any build targets, including intermediate ones

.SECONDARY:	$(CLEAN)

#
# Stuff for the top level directory
#
TOP_SRC     := UModel.pas UDraw.pas UData.pas UXfit.pas UConsole.pas \
               UOptions.pas UVector.pas UConsole.xfm UData.xfm UModel.xfm \
               UOptions.xfm USelect.xfm xfit.dpr DrawWindow.res \
               UDLL.pas \
               DrawWindow.dpk Xfit.res

xfit: $(TOP_SRC) $(TGT_LIB)
	dcc xfit
#

