# [Enjector][1] [![MIT License][license-badge]](LICENSE.md)

[![Join the chat at https://gitter.im/ci_helloworld/Lobby][gitter-badge]][gitter-link]

[![Build Status][travis-badge]][travis-link]
[![codecov][codecov-badge]][codecov-link]
[![coverity][coverity-badge]][coverity-link]


Welcome to the Enjector source code tree!

Enjector is a high performance, scalable machine learning and complex event processing platform. 
With this engine you can build intelligent applications both in the cloud and on edge devices.

The types of problems you can solve:
- Statistically compress raw sensor signal data using pattern recognition from the edge (IoT devices), which means less data/bandwidth required sending meaningful domain events to your cloud.
- Detect certain patterns or anomalies and create actionable insights from domain events. 

![async-await](doc/img/enjector-overview.png "Enjector Overview")

It is architected with three key objectives:

- To create a high-performance, embeddable engine for low powered IoT (Internet of Things) devices.
- Process a high volume of IoT events meet the needs of today's web-scale applications.
- To provide the robustness and reliability through clustering.
- To provide operational efficiency (minimal manual involvement).
- Written in pure C99 with minimal/no-dependency - designed to be lightening fast and light weight. A lot of attention is paid to how the CPU, memory and I/O is used efficiently.

This is our target architecture:

![async-await](doc/img/enjector-architecture.png "Enjector Architecture")

For more information on Enjector, please visit: [`http://enjector.com`](http://enjector.com)

## Build Prerequisites

The Enjector can be built and deployed on various current ARM, 32/64-bit Linux, 32/64-bit Windows 10/2016 and MacOSX platforms.

### Dependencies

All of Enjector source code is written in the C programming language, conforming to the ANSI C99 standard. It doesn't require any other dependency apart from these basic build tools:

In particular, the following tools and libraries are needed:

#### C Compiler Toolchain for UNIX/MacOSX environments

Building Enjector on UNIX/MacOSX environment requires GCC 5 or later C compiler toolchain, 
with the standard GNU/Linux development tools and libraries installed in the build environment, including:

* `cmake`
* `valgrind` (optional for memory checks using `$ make memory_check`)
* `gcov` (optional for coverage checks using `$ make coverage`)
* `gcovr` (optional for coverage checks using `$ make coverage`)
* `cppcheck` (optional for static analysis using `$ make static_check`)

#### C Compiler Toolchain for Windows environments

Building Enjector on Windows environment requires Visual Studio 2017 with the following features enabled:

* `Desktop development with C++`
* `Visual C++ tools for CMake`

## Building Enjector

### Default Build on UNIX/MacOSX

	$ cmake .       -- Creates release version of Makefiles.
	$ make          -- Perform the release build of the main server, core unit tests and core benchmark tests.

Or Debug versions

	$ cmake -DCMAKE_BUILD_TYPE=Debug .     -- Create debug versions of Makefiles.
	$ make          -- Perform the debug build of the main server, core unit tests and core benchmark tests.

### To run Enjector

	$ ./enjector

Open a browser to the location http://localhost:8888

### To run unit tests

	$ ./enjector-core-tests

### To run benchmark tests

	$ ./enjector-benchmark-tests

### To run memory leak check
	$ make memory_check

### To run coverage check
	$ make coverage
A detailed HTML coverage report is available in the directory analysis/coverage/ 

### To run static analysis
	$ make static_check
A detailed HTML static analysis report is available in the directory analysis/static 

### Quick update, build and check
    $ git pull && make clean && cmake . && make && make coverage && make static_check && make memory_check 

# Status/Roadmap

- June 2018: Event - started clean room implementation of core in GitHb as open source.
- June 2018: Plan - complete core Verbs Engine implementation.
- July-August 2018: Plan - complete core Workflow Engine and Designer implementation.

[1]: https://enjector.com/
[travis-badge]:    https://travis-ci.org/enjector/enjector.svg?branch=master
[travis-link]:     https://travis-ci.org/enjector/enjector

[license-badge]:   https://img.shields.io/badge/License-Apache%202.0-blue.svg

[codecov-badge]:   https://codecov.io/gh/enjector/enjector/branch/master/graph/badge.svg
[codecov-link]:    https://codecov.io/gh/enjector/enjector

[coverity-badge]:  https://img.shields.io/coverity/scan/16063.svg
[coverity-link]:   https://scan.coverity.com/projects/enjector-enjector

[gitter-badge]:  https://badges.gitter.im/enjector/Lobby.svg
[gitter-link]:   https://gitter.im/enjector/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge
