# README for Nymph/Testing

## C++

### Testing infrastructure: Catch2

Nymph uses the Catch2 testing framework for unit tests.  See these links for more information:

* [Main GitHub page](https://github.com/catchorg/Catch2)
* [Tutorial](https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md)

### Building tests

In CMake, enable the option `Nymph_ENABLE_TESTING`, and build.

The testing executable, `RunTests`, will be installed in `install_prefix/bin`.

### Running tests

To see the available options for running tests, you can do:

```
> bin/RunTests -h
```

To simply run all of the tests, you can do:

```
> bin/RunTests
```

Or if you want to run a specific test, you can do:

```
> bin/RunTests [name]
```

For further documentation on using Catch2, see the [tutorial](https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md).

## Python

Python testing is divided into two sections: binding and nymph.  The former tests the C++-to-python binding and doesn't 
require the Python `nymph` package. Those tests will import the `nymph_bindings` package.  It requires having built the 
C++ side of Nymph with the `Nymph_ENABLE_PYTHON` flag set to `TRUE`.

The nymph tests test the Python `nymph` package and requires having installed the nymph package (e.g. with `pip install`).  
If the package is not installed in a system location, you'll need to source `add_lib_python_path.sh` before running the tests.

### Testing infrastructure: unittest



### Binding tests

#### Running tests

Individual tests can be run by specifying the module name or the python file.  
From the CL, in the `Testing/Python` directory, you can run:

```
> python -m unittest Bindings/test[name].py
```

