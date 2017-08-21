               Release Notes for XFIT Version 1.2 (beta)
               -----------------------------------------

    Please note that this is "beta" software.  No guarantees are made as to
    usability.  Any problems and issues you have should be communicated to
    James Hester (jrh@anbf2.kek.jp)

                        Introduction

XFit version 1.1 was released in 1996 with support for FEFF4 and FEFF6
routines.  The world has moved on, and FEFF8.2 is now the standard,
with FEFF4 no longer available for download.  The underlying code of
XFit, written in Delphi and C/C++, was also showing its age.  This
release of XFit is primarily a "bug-fix" release, with few
user-visible changes, in order to update the code after 7 years of
neglect.

                        Changes from Version 1.1

1.  Naming change.  To avoid confusion, "XFit" now refers to the EXAFS 
refinement component of the set of programs formerly also known as "XFit". 
This set of programs is now called the "XFit Suite".

2.  FEFF version.  XFit version 1.2 supports FEFF8 running in
"FEFF6" mode (command: "theory feff8"), that is, doing a multiple
scattering calculation in the same way as FEFF6 does.  Useful features
of FEFF8, such as calculation of E0 and S0, have not yet been included
into XFit.  The FEFF4 model, while still available, is no longer
actively supported.  A future release of XFit will provide a
"single-scattering" model implemented using FEFF8 "OVERLAP" and "SS"
cards.

3.  Cross platform.  With the rise in popularity of Linux, the XFit graphical
interface has been adapted to run on both Windows and Linux platforms.  This
has led to small changes in the behaviour of the Xfit console: please report
any problems you have.

4.  Bugs removed: an intermittent show-stopping bug in the console on Win98/
Win2000 platforms seems to have disappeared in this release.

5.  Help unavailable: the help file format was out of date and is being
moved into a portable HTML.  The new files are not yet available.  Please use
the help from XFit v1.1.

                         Installing FEFF

**Important**

You should download the FEFF executables from the University of Washington
yourself, and put them into a directory that is on the system path.  In
older versions of Windows, this path is set in the "Autoexec.bat" file. In
newer versions, Settings->System->Advanced will take you to a place where
you can set your path.

For FEFF8, the executable should be called "feff820" (Linux/Windows) or 
"feff820.exe" (Windows only).  For FEFF6, it should be called "feff6" 
or "feff6.exe"

                         Known Bugs in this release
               
1. Use of up and down arrow keys to retrieve previous lines doesn't always
work

2. Printing is not available

3. Online help is missing.

4. Active area of console doesn't occupy full window.
