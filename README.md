Nymph
=====

Nymph is a C++-based data analysis framework.


Dependencies
------------

- CMake (3.12 or better)
- Boost (date_time, filesystem, program_options, system, thread; 1.46 or better)
- RapidJSON (optional; required for JSON processing)
- yaml-cpp (optional; required for YAML processing)


Operating System Support
------------------------

* Mac OS X (usually tested on OS X 10.10)
* Linux (usually tested on Ubuntu)


Directory Structure
-------------------

*  cmake - Files that CMake uses to build KTCore.
*  Documentation - Library documentation
*  Executables - Source code for the main Nymph executable and test programs.
*  Library - Source for the Nymph library
*  Scarab - Submodule
*  Templates - Example files for creating a processor and the main CMakeLists.txt file for a package using Nymph.


Installing
----------


Instructions for Use
--------------------


Documentation
-------------



Development
-----------

The Git workflow used is git-flow:
* http://nvie.com/posts/a-successful-git-branching-model/
We suggest that you use the aptly-named git extension, git-flow, which is available from commonly-used package managers:
* https://github.com/nvie/gitflow

Issues should be posted via [GitHub](https://github.com/project8/nymph/issues).