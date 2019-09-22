# bflow
A proof-of-concept framework for composing FSM-like procedure flows

## Using the framework

For examples on how to use the framework please see [the tests](src/test/flow_tests.cpp).

## Building

To build the project you will need [CMake](https://cmake.org/), and a C++ compiler that supports C++17 standard.

Once you have the repository checked out,
create a build directory, 
run cmake in it (cmake will attempt to download [Googletest](https://github.com/google/googletest) which is used as a testing framework in this project),
make, and (optionally) run tests:

```bash
# in the project's main directory
mkdir build
cd build
cmake ..
make
./unit_tests
```
