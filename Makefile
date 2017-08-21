# Makefile for XFit on Linux, using Delphi
#
CC              = build/ccd-gcc
C++             = build/ccd-gcc
COMP            = $(CC) $(CF_ALL) $(CF_TGT) -o $@ -c $<
C++COMP         = $(C++) $(CF_ALL) $(CF_TGT) -o $@ -c $<
LINK            = $(CC) $(LF_ALL) $(LF_TGT) -o $@ $^ $(LL_TGT) $(LL_ALL)
COMPLINK        = $(CC) $(CF_ALL) $(CF_TGT) $(LF_ALL) $(LF_TGT) -o $@ $< $(LL_TGT) $(LL_ALL)
INST            = /usr/bin/install

DIR_BIN = $(DESTDIR)/usr/bin
DIR_LIB = $(DESTDIR)/usr/lib

include Rules.mk
