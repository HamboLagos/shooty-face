# Installation
* Follow cmake tutorial to build and install, overriding a few options. These
should work fine for Linux, refer to docs for other OS's or building for
distribution
    * C_COMPILER and CXX_COMPILER to clang and clang++
    * Install location to local path
    * Choose "Release" or "Debug" build as desired
    * set options for building docs and examples as desired
* `make install` copies the dynamic libs, headers, and docs/examples to install
location
    * Move FindSFML.cmake and headers to working dir
    * Add built libs to library path
