# Microsoft Developer Studio Project File - Name="XFIT_WIN32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XFIT_WIN32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XFIT_WIN32.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XFIT_WIN32.MAK" CFG="XFIT_WIN32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XFIT_WIN32 - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFIT_WIN32 - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XFIT_WIN32 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /Zp1 /MT /W3 /vmb /vd1 /GX /Zi /Od /I "..\..\..\include" /I "..\..\..\includex" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0xc09 /d "NDEBUG"
# ADD RSC /l 0xc09 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes /machine:I386

!ELSEIF  "$(CFG)" == "XFIT_WIN32 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /Zp1 /MTd /W3 /Gm /GX /Zi /Od /I "..\..\..\include" /I "..\..\..\includex" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0xc09 /d "_DEBUG"
# ADD RSC /l 0xc09 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /debugtype:coff /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XFIT_WIN32 - Win32 Release"
# Name "XFIT_WIN32 - Win32 Debug"
# Begin Source File

SOURCE=..\actions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\anbf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\argument.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\argument.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ascii_da.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ascii_da.h
# End Source File
# Begin Source File

SOURCE=..\c_constr.c
# End Source File
# Begin Source File

SOURCE=..\c_constr.h
# End Source File
# Begin Source File

SOURCE=..\calculat.c
# End Source File
# Begin Source File

SOURCE=..\calculat.h
# End Source File
# Begin Source File

SOURCE=..\char_set.c
# End Source File
# Begin Source File

SOURCE=..\char_set.h
# End Source File
# Begin Source File

SOURCE=..\commands.c
# End Source File
# Begin Source File

SOURCE=..\commands.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\comment.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\comment.h
# End Source File
# Begin Source File

SOURCE=..\common.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\complex.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\complex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\console.h
# End Source File
# Begin Source File

SOURCE=..\console.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\constant.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\constant.h
# End Source File
# Begin Source File

SOURCE=..\cvector.cxx
# End Source File
# Begin Source File

SOURCE=..\cvector.h
# End Source File
# Begin Source File

SOURCE=..\data.cxx
# End Source File
# Begin Source File

SOURCE=..\data.h
# End Source File
# Begin Source File

SOURCE=..\ddata.h
# End Source File
# Begin Source File

SOURCE=..\ddatadll.cxx
# End Source File
# Begin Source File

SOURCE=..\debug.c
# End Source File
# Begin Source File

SOURCE=..\debug.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\draw_m.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\dsrs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\exafs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\exafs.h
# End Source File
# Begin Source File

SOURCE=..\exafs_ty.c
# End Source File
# Begin Source File

SOURCE=..\exafs_ty.h
# End Source File
# Begin Source File

SOURCE=..\expressi.c
# End Source File
# Begin Source File

SOURCE=..\expressi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\feff4.c
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\feff4.h
# End Source File
# Begin Source File

SOURCE=..\feff6.c
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\feff6.h
# End Source File
# Begin Source File

SOURCE=..\feff6.h
# End Source File
# Begin Source File

SOURCE=..\feff8.c
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\feff8.h
# End Source File
# Begin Source File

SOURCE=..\feff8.h
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\feff_.c
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\feff_.h
# End Source File
# Begin Source File

SOURCE=..\feff_406.c
# End Source File
# Begin Source File

SOURCE=..\feff_406.h
# End Source File
# Begin Source File

SOURCE=..\feff_601.c
# End Source File
# Begin Source File

SOURCE=..\feff_601.h
# End Source File
# Begin Source File

SOURCE=..\feff_68.c
# End Source File
# Begin Source File

SOURCE=..\feff_68.h
# End Source File
# Begin Source File

SOURCE=..\feff_820.c
# End Source File
# Begin Source File

SOURCE=..\feff_820.h
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\feff_ms.c
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\feff_ms.h
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\ff4_chi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\ff4_chi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\ff6_chi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\ff6_chi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\ff8_chi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\includex\ff8_chi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\fft.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\fft.h
# End Source File
# Begin Source File

SOURCE=..\filter.c
# End Source File
# Begin Source File

SOURCE=..\filter.h
# End Source File
# Begin Source File

SOURCE=..\fourier_.c
# End Source File
# Begin Source File

SOURCE=..\fourier_.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gauss.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gauss.h
# End Source File
# Begin Source File

SOURCE=..\global.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\grad.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\grad.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\header.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\header.h
# End Source File
# Begin Source File

SOURCE=..\include.c
# End Source File
# Begin Source File

SOURCE=..\include.h
# End Source File
# Begin Source File

SOURCE=..\input.c
# End Source File
# Begin Source File

SOURCE=..\input.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\interpol.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\interpol.h
# End Source File
# Begin Source File

SOURCE=..\lex_yy.c
# End Source File
# Begin Source File

SOURCE=..\list_com.c
# End Source File
# Begin Source File

SOURCE=..\list_com.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\make_w.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\matrix.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\matrix.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\midas.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\minimise.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\minimise.h
# End Source File
# Begin Source File

SOURCE=..\misc_com.c
# End Source File
# Begin Source File

SOURCE=..\misc_com.h
# End Source File
# Begin Source File

SOURCE=..\models.c
# End Source File
# Begin Source File

SOURCE=..\models.h
# End Source File
# Begin Source File

SOURCE=..\montecar.c
# End Source File
# Begin Source File

SOURCE=..\montecar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\noise.h
# End Source File
# Begin Source File

SOURCE=..\paramete.c
# End Source File
# Begin Source File

SOURCE=..\paramete.h
# End Source File
# Begin Source File

SOURCE=..\parse_da.c
# End Source File
# Begin Source File

SOURCE=..\parse_da.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\poly.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\print.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\print.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ps.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ps_plot.h
# End Source File
# Begin Source File

SOURCE=..\read_exa.c
# End Source File
# Begin Source File

SOURCE=..\read_exa.h
# End Source File
# Begin Source File

SOURCE=..\refine.c
# End Source File
# Begin Source File

SOURCE=..\refine.h
# End Source File
# Begin Source File

SOURCE=..\refine_c.c
# End Source File
# Begin Source File

SOURCE=..\refine_c.h
# End Source File
# Begin Source File

SOURCE=..\restrain.c
# End Source File
# Begin Source File

SOURCE=..\restrain.h
# End Source File
# Begin Source File

SOURCE=..\save_exa.c
# End Source File
# Begin Source File

SOURCE=..\save_exa.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\scan.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\scan.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\search.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\search.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\statisti.c
# End Source File
# Begin Source File

SOURCE=..\..\..\include\statisti.h
# End Source File
# Begin Source File

SOURCE=..\stringextra.cxx
# End Source File
# Begin Source File

SOURCE=..\stringextra.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\symmetry.h
# End Source File
# Begin Source File

SOURCE=..\x_curves.c
# End Source File
# Begin Source File

SOURCE=..\x_curves.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\xas.h
# End Source File
# Begin Source File

SOURCE=..\xconsole.c
# End Source File
# Begin Source File

SOURCE=..\xdraw_mo.c
# End Source File
# Begin Source File

SOURCE=..\xdraw_mo.h
# End Source File
# Begin Source File

SOURCE=..\xdraw_xa.c
# End Source File
# Begin Source File

SOURCE=..\xfit.c
# End Source File
# Begin Source File

SOURCE=..\xfit_win32dll.cxx
# End Source File
# Begin Source File

SOURCE=..\xfity_tab.c
# End Source File
# Begin Source File

SOURCE=..\xfity_tab.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\xfpakg.h
# End Source File
# Begin Source File

SOURCE=..\xfpakg_c.c
# End Source File
# Begin Source File

SOURCE=..\xfpakg_c.h
# End Source File
# End Target
# End Project
