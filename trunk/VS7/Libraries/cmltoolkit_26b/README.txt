
		Vortex Simulation Toolkit Version 2.3
		-------------------------------------

		       CMLabs Simulations, Inc.


NOTE: Please read the license agreement in the file named LICENSE before 
using any of the software in this distribution.

The Vortex Simulation Toolkit includes the following components:

    MST - Simulation Tools Module provides the main entry point to
	  VORTEX and a bridge between collision detection and dynamics
	  tools.

    MDT - Dynamics Tools Module provides complete specification of the
	  physical interactions between objects and joints and their
	  environment.

    MCD - Collision Detection Module contains collision detection and
	  contact creation algorithms, as well as related utility
	  functions.  

    Viewer - a light-weight graphics API that uses OpenGL or DirectX
         for rendering. The Viewer is included to provide basic
         rendering and a user interface for the example programs.

    Libraries for easy integration with various graphics libraries.

    Vx - a pre-release version of the upcoming C++ api, which will
         enhance the graphics library integrations and simply
         the use of Vortex.

Source code for examples and tutorial programs is provided in this
package.  Vortex documentation, and pre-built executables for the
examples and tutorials are available in separate downloads.

The target runtime and developer platforms for these toolkits are
Windows NT, Windows 2000, Windows XP, Irix, and Linux.

Note that because Windows NT only supports DirectX up to version 3.0,
it will only run the example programs under OpenGL.  If you install
the DirectX SDK (version 7.0 or better) on Windows NT 4.0, you will be
able to compile MeViewer2 with DirectX 7 support, but the executables
will not run under Windows NT.


Downloading Vortex
------------------

Platform-specific archives are available for download from the downloads
page in the member area of the CMLabs website at http://www.cm-labs.com

Untar or unzip the appropriate archive file for your platform into an 
empty directory.


Documentation
-------------

The documentation for Vortex is now a separate download. The documentation
package is designed to be unpacked into the same directory where you
unpacked the toolkit.

The documentation is browser based, and consists of PDF and HTML files.
You will need a Web browser and Adobe Acrobat Reader.  You can download
Acrobat Reader from http://www.adobe.com.
 
After downloading and unpacking the documentation package, open
cmltoolkit/doc/index.html to view the documentation.

The documentation includes:

  - Vortex User Guide (pdf)
  - Vortex Simulation Reference Manual
  - Vortex Dynamics Reference Manual
  - Vortex Collision Reference Manual
  - Vortex Viewer Developer Guide (pdf)
  - Utility Reference Manuals
        * Definitions & Tools
        * Demo support
  - File Format Developer Guide (pdf)
  - Demo Documentation
  - Release Notes

Please refer to the CMLabs website (http://www.cm-labs.com) for additional 
or updated documentation.


Example and Tutorial Programs
-----------------------------

After you download and unpack the examples and tutorials package, the
bin directory will contain executables for all examples and
tutorials.

For the example programs 'F1' displays/hides Help.


Vortex Layout
-------------
  - cmltoolkit/bin		: pre-compiled executables for examples
				  and tutorials (separate download)

  - cmltoolkit/doc		: documentation (separate download)

  - cmltoolkit/include		: Vortex include files.

  - cmltoolkit/lib.rel		: release libraries, single and double precision.

  - cmltoolkit/lib.chk		: check libraries, single and double precision.
 
  - cmltoolkit/src/components	: contains subdirectories for each module for
				  which source code is provided.
        - Mst			  : Vortex Simulation Tools
        - Mdt			  : Vortex Dynamics 
        - Mcd			  : Vortex Collision 
        - MeApp			  : Support examples and tutorials
        - MeGlobals		  : Utilities and Global Includes
        - MeViewer2		  : Vortex Renderer
        - MeFile		  : XML file handling routines
	- McdFrame		  : Vortex Collision Framework
        - McdWTK                  : World Toolkit integration
        - McdPerformer            : Performer integration
        - Vx                      : prerelease C++ API.

  - cmltoolkits/src/examples	: sources for examples

  - cmltoolkits/src/tutorials	: sources for tutorials


Contacts
--------

CMLabs website:  http://www.cm-labs.com

Email:   sales@cm-labs.com    - sales information and general inquiries
	 support@cm-labs.com  - support for Vortex
	 licman@cm-labs.com   - licensing issues
