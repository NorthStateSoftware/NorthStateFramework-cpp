NorthStateFramework-cpp
=======================

An object-oriented framework for implementing UML state machines.  For more information, see the pages <a href="http://northstatesoftware.github.io/NorthStateFramework-cpp/index.html">here.</a>



1. [Compilation](#compilation)
2. [Using NSF](#using-nsf)
3. [Documentation](#documentation)
4. [Contributing](#contributing) 

Compilation
===========

a. Build Requirements
---------------------
* The solution provided is compatible with Visual Studio 2013.
* <a href="http://www.doxygen.org/">Doxygen</a> is required to build the documentation. (Optional)

b. Building the Software using Visual Studio
----------------------------
* Open the file NorthStateFramework-cpp.sln within Visual Studio.
* Choose either Debug or Release build configurations.
* Build the code from the IDE
* Output will be placed in the Build directory (either Debug or Release)
	* Executables will be created for the test project and each of the example projects
	* NorthStateFramework.lib will be created for use in other projects.

c. Building the Software using Cmake
----------------------------
* Assuming you are currently in the NSF source directory on a Unix machine
* Create build directory
  * mkdir cmake_build && cd cmake_build
* Build only static library in Release configuration
  * cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
  * make
  * make install
* Build Debug configuration with tests and examples
  * cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON ..
  * make
  * make install
* Build Debug configuration with tests, examples and address sanitizer support for stack buffer overflow and memory leaks detection (libasan required)
  * cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DUSE_ADDRESS_SANITIZER=ON ..
  * make
  * make install
* Output will be placed in the Build directory in "dist" subdirectory
  * Executables will be created for the test project and each of the example projects  
  * northstateframework.a will be created for use in other projects.  

By default Cmake will detect windows/linux systems and include appropriate OSPorts modules, but you can
specify system name manually.
Example for POSIX-compatible systems: cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DCMAKE_SYSTEM_NAME=POSIX ..
Example for eCOS system: cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DBUILD_EXAMPLES=ON -DCMAKE_SYSTEM_NAME=eCOS ..

d. Building the Documentation
-----------------------------
* If you wish to build the documentation you must first install Doxygen.
* Open the solution
* Right click on the Documentation project and choose build.
* Index.html and other html pages will be generated to the \Documentation\Doxygen\HTML directory.

Using NSF
=========
NSF is a framework contained in a .lib for inclusion in projects that require state machine behavior.  To start using  NSF, link against the NorthStateFramework.lib within your project.  See the included examples.  For an in-depth tutorial on using NSF, see the WorkingWithNorthStateFramework document contained in the solution.

Documentation
=============
If you would rather not generate the documentation, you can use the online documentation located at http://northstatesoftware.github.io/NorthStateFramework-cpp/Documentation.

Contributing
============
Before submitting changes to NorthStateFramework-cpp, please review the contribution guidelines at https://github.com/NorthStateSoftware/NorthStateFramework-cpp/wiki/Contributor-FAQ.
