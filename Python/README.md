Nymph
=====

Nymph is a Python extension of the C++-based Nymph data analysis framework.


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

*  nymph - nymph modules.

Installing
----------
To build the python package use

python -m build 

To install and build the python package use

pip install .

in the directory where the setup.py script is located.

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