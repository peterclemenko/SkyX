--------------------------------------------------------------------------------
Copyright (C) 2011 Nodrev <jeffnodrev@gmail.com>

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
--------------------------------------------------------------------------------

CMake build system for Hydrax 0.2.
You need to define an "OGRE_HOME" environment variable pointing to the directory where OGRE sdk is installed.

Options available:
 - SKYX_BUILD_SAMPLES: build skyx samples or not
 - SKYX_INSTALL_SAMPLES: install samples in the skyx generated sdk dir
 - SKYX_INSTALL_PDB (windows only): install debug symbols files (pdb files) to the skyx generated sdk dir
 - SKYX_STATIC: build Skyx as a static lib.
 - SKYX_DEPENDENCIES_DIR: optionnal entry for dependencies checking, if the cmake system do not find dependencies automatically.

Notes concerning static building:
Ogre must be compiled as a static lib too in order to activate this option.
You will have to modify Prerequisites.h slightly with the following code:
// -----------------------------------------------------------------------------
// Define the dll export qualifier if compiling for Windows
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   if defined(SKYX_STATIC_LIB)
#       define DllExport
#   else
#       ifdef SKYX_LIB
#           define DllExport __declspec (dllexport)
#       else
#           define DllExport __declspec (dllimport)
#       endif
#   endif
#else
#	define DllExport
#endif
// -----------------------------------------------------------------------------
Furthemore, you'll have to add a manual boolean entry named OGRE_STATIC (obviously, it's value must be "TRUE").
Finally, Demos are may not compile/run cleanly, so it is recommended to disable sample build while using static building.
