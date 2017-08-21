#
# Adapted makefile found in Win 32 directory 
#
CC  = gcc
CXX = g++
CPPFLAGS =  -DUSE_GRAPH -I../../include -I../../includex
CFLAGS =  -g  -Wall
CXXFLAGS = -g 
LD = ld
LDFLAGS =  --shared
FLEX = flex
BISON = bison
#
LIBRARIES = -lstdc++ cvector.o data.o ddatadll.o xfitdll.o 
#
OBJECTS = ../../include/argument.o  ../../include/ascii_da.o  ../../include/comment.o \
          ../../include/complex.o   ../../include/constant.o  ../../include/exafs.o   \
          ../../include/fft.o       ../../include/gauss.o     ../../include/grad.o    \
          ../../include/header.o    ../../include/interpol.o  ../../include/matrix.o  \
          ../../include/minimise.o  ../../include/print.o     ../../include/scan.o    \
          ../../include/search.o    ../../include/statisti.o  ../../includex/feff4.o  \
          ../../includex/feff_.o    ../../includex/feff_ms.o  ../../includex/ff8_chi.o \
          ../../includex/ff4_chi.o  ../../includex/ff6_chi.o  calculat.o           \
          commands.o             c_constr.o             feff6.o   \
          exafs_ty.o             expressi.o             filter.o             \
          fourier_.o             include.o              input.o              \
          list_com.o             models.o                  \
          montecar.o             paramete.o             parse_da.o           \
          read_exa.o             refine.o               refine_c.o           \
          restrain.o             save_exa.o             xfit.o               \
          x_curves.o             xfity_tab.o            feff_601.o           \
          feff_406.o             xfpakg_c.o             lex_yy.o             \
          feff_820.o             feff8.o                feff_68.o \
          char_set.o             stringextra.o      \
          misc_com.o      #       draw_3d.o
#          draw.o                 draw_mod.o             draw_sys.o           \
#          draw_win.o             draw_xas.o 
#  colour_s.o   event.o
#
#  Note that I have excluded misc_com.c, which has a lot of Windows specific communication
#  stuff in it
#
#
#  We define a rule for cxx, as that is what bloody Windows thinks a C++ file suffix is
#
%.o: %.cxx
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

EXTRAS = xconsole.o xdraw_mo.o xdraw_xa.o

libxfit.so: xfity_tab.h $(OBJECTS) $(LIBRARIES) $(EXTRAS)
	$(CC) -g $(LDFLAGS) -o libxfit.so $(OBJECTS) $(EXTRAS) $(LIBRARIES)
#
# the underscore is to maintain windows compatibility
#
lex_yy.c: xfitl.l input.h xfity_tab.c xfity_tab.h
	$(FLEX) -i -I -L -8 -t xfitl.l > lex_yy.c
#
xfity.tab.c: xfity.y
	$(BISON) -d xfity.y
#
#  again, for windows compatibility
#
xfity_tab.c: xfity.tab.c
	mv xfity.tab.c xfity_tab.c
#
xfity_tab.h: xfity.tab.c
	mv xfity.tab.h xfity_tab.h
#
#
clean:
	rm -fr *.o
	rm -fr xfity.tab.c xfity.tab.h lex_yy.c lex.yy.c xfity_tab.h xfity_tab.c
	rm -fr ../../include/*.o
	rm -fr ../../includex/*.o
	rm -fr libxfit.so
