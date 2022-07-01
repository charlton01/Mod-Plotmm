# Mod-Plotmm
Update of Plotmm, a gtkmm plotting library, to gtkmm-3.0.  The original git submission was in 2015.  The code was subsequently updated as of December 6, 2019.  OS version Linux Ubuntu 18.04; Gtkmm-3.0 version 3.22.2-2.

Mod-Plotmm

Plotmm is a plotting library for use with Gtk and Gtkmm.  The original code was written by Andy Thaller and can be found at http://plotmm.sourceforge.net/.  The last update of the original code was in 2005 and this useful plotting libary unfortunately no longer compiles with gtkmm-3.0.

In order to make the library compatible with gtkmm-3.0, the code was modified and rewritten.  The majority of the original design and functionality remain, although the code has been edited heavily in some places.

No change to the original software license is intended in this revision and the original copyright notices of Andy Thaller remain in the code.

PlotMM Widget Library
 * Copyright (C) 2004   Andy Thaller
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the LGPL


Information regarding the changes made to the original code can be obtained from Jim Charlton, charltn@gmail.com

The package provided is configured for compilation on the Linux (Ubuntu) OS.  I am using version 14.01.1 of Ubuntu. The Makefile has two targets (debug and release).  "debug" will produce a small test program, bin/Debug/myprog, which will test the code in a simple program.  The code that is executed is in main.cc.  The "release" target will generate the library bin/Release/plotmm.so that you can link it to your code as a librarylibgtkmm-3.0-dev.

I have also installed gtkmm/osx on the MAC OS X (version 10.10) and this code also compiles on that system.  Information on compling on MAC OS X can be obtained by writing to me (charltn@gmail.com).  On December 6, 2019, the code was compiled on MacOS 10.15 (Catalina) without issues.

The current date is June 30, 2022.  The code still compiles on Ubuntu 22.04.  To compile, install the gtkmm libraries with 'apt install libgtkmm-3.0-dev' (as root user) then run 'make release' or 'make debug'.
