#
# Adapted makefile found in Win 32 directory, changed to non-recursive
# method 
#
# Standard things

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)


CXX = g++
CXXFLAGS = -g 
FLEX = flex
BISON = bison
#
OBJS_$(d) := $(d)/../include/argument.o  $(d)/../include/ascii_da.o  $(d)/../include/comment.o \
          $(d)/../include/complex.o   $(d)/../include/constant.o  $(d)/../include/exafs.o   \
          $(d)/../include/fft.o       $(d)/../include/gauss.o     $(d)/../include/grad.o    \
          $(d)/../include/header.o    $(d)/../include/interpol.o  $(d)/../include/matrix.o  \
          $(d)/../include/minimise.o  $(d)/../include/print.o     $(d)/../include/scan.o    \
          $(d)/../include/search.o    $(d)/../include/statisti.o  $(d)/../includex/feff4.o  \
          $(d)/../includex/feff_.o    $(d)/../includex/feff_ms.o \
          $(d)/../includex/ff4_chi.o  $(d)/../includex/ff6_chi.o  $(d)/calculat.o           \
          $(d)/../includex/ff8_chi.o            \
          $(d)/commands.o             $(d)/c_constr.o             $(d)/feff6.o   \
          $(d)/exafs_ty.o             $(d)/expressi.o             $(d)/filter.o             \
          $(d)/fourier_.o             $(d)/include.o              $(d)/input.o              \
          $(d)/list_com.o             $(d)/models.o                  \
          $(d)/montecar.o             $(d)/paramete.o             $(d)/parse_da.o           \
          $(d)/read_exa.o             $(d)/refine.o               $(d)/refine_c.o           \
          $(d)/restrain.o             $(d)/save_exa.o             $(d)/xfit.o               \
          $(d)/x_curves.o             $(d)/xfity_tab.o            $(d)/feff_601.o           \
          $(d)/feff_406.o             $(d)/xfpakg_c.o             $(d)/lex_yy.o             \
          $(d)/feff_820.o  $(d)/feff8.o  $(d)/feff_68.o \
          $(d)/char_set.o             $(d)/stringextra.o      \
          $(d)/cvector.o $(d)/data.o $(d)/ddatadll.o $(d)/xfitdll.o \
          $(d)/xconsole.o $(d)/xdraw_mo.o $(d)/xdraw_xa.o \
          $(d)/misc_com.o      #       draw_3d.o
#          draw.o                 draw_mod.o             draw_sys.o           \
#          draw_win.o             draw_xas.o 
#  colour_s.o   event.o
#
#  Note that I have excluded misc_com.c, which has a lot of Windows specific communication
#  stuff in it
#
DEPS_$(d)	:= $(OBJS_$(d):%=%.d)

$(OBJS_$(d)): CF_TGT := -I$(d) -g -I$(d)/../include -I$(d)/../includex
$(OBJS_$(d)): $(d)/xfity_tab.h

TGTS_$(d):= $(d)/libxfit.so.0
TGT_LIB := $(TGT_LIB) $(TGTS_$(d))

CLEAN := $(CLEAN) $(d)/*.o $(d)/../include/*.o $(d)/../includex/*.o $(TGTS_$(d))
CLEAN := $(CLEAN) $(d)/xfity.tab.c $(d)/xfity.tab.h $(d)/lex_yy.c $(d)/lex.yy.c
CLEAN := $(CLEAN) $(d)/xfity_tab.c $(d)/xfity_tab.h $(DEPS_$(d))

$(TGTS_$(d)): LF_TGT := -g --shared -Wl,-soname=libxfit.so.0
$(TGTS_$(d)): LL_TGT := -lstdc++
$(TGTS_$(d)): $(OBJS_$(d))
	$(LINK)
#
$(d)/lex_yy.c: $(d)/xfitl.l $(d)/input.h $(d)/xfity_tab.c $(d)/xfity_tab.h
	$(FLEX) -i -I -L -8 -t $< > $@
#
$(d)/xfity.tab.c $(d)/xfity.tab.h: $(d)/xfity.y
	$(BISON) -d -o $(<D)/xfity.tab.c $<
#
$(d)/xfity_tab.c: $(d)/xfity.tab.c
	mv $< $@
#
$(d)/xfity_tab.h: $(d)/xfity.tab.h
	mv $< $@

# Standard things to finish up
-include $(DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))
