# CCSys

[![Build Status](https://travis-ci.org/marcomaggi/ccsys.svg?branch=master)](https://travis-ci.org/marcomaggi/ccsys)
[![Coverity passed](https://scan.coverity.com/projects/13048/badge.svg)](https://scan.coverity.com/projects/marcomaggi-ccsys)
[![codecov](https://codecov.io/gh/marcomaggi/ccsys/branch/master/graph/badge.svg)](https://codecov.io/gh/marcomaggi/ccsys)


## Introduction

This package  installs a  standard C11  language library;  it implements
adapters  for POSIX  system  functions to  convert  the error  reporting
mechanism to the use of the library CCExceptions.

This  package depends  on  the package  CCExceptions,  at least  version
`0.7.1-beta.1`.

The  package makes  use of  the  GNU Autotools  and  it is  tested on  a
GNU+Linux system.


## License

Copyright (c) 2017, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

This program is free software: you  can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by
the Free  Software Foundation, either version  3 of the License,  or (at
your option) any later version.

This program  is distributed  in the  hope that it  will be  useful, but
WITHOUT   ANY   WARRANTY;  without   even   the   implied  warranty   of
MERCHANTABILITY  or  FITNESS FOR  A  PARTICULAR  PURPOSE.  See  the  GNU
General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.


## Install

To install from a proper release tarball, do this:

```
$ cd ccsys-0.1.0
$ mkdir build
$ cd build
$ ../configure
$ make
$ make check
$ make install
```

to inspect the available configuration options:

```
$ ../configure --help
```

The script offers the following special options:

* `--enable-glibc` Enable GNU C Library features (default is yes).

* `--enable-linux` Enable Linux features (default is yes).

The Makefile is designed to allow parallel builds, so we can do:

```
$ make -j4 all && make -j4 check
```

which,  on  a  4-core  CPU,   should  speed  up  building  and  checking
significantly.

The Makefile supports the DESTDIR  environment variable to install files
in a temporary location, example: to see what will happen:

```
$ make -n install DESTDIR=/tmp/ccsys
```

to really do it:

```
$ make install DESTDIR=/tmp/ccsys
```

After the  installation it is  possible to verify the  installed library
against the test suite with:

```
$ make installcheck
```

From a repository checkout or snapshot  (the ones from the GitHub site):
we  must install  the GNU  Autotools  (GNU Automake,  GNU Autoconf,  GNU
Libtool), then  we must first run  the script "autogen.sh" from  the top
source directory, to generate the needed files:

```
$ cd ccsys
$ sh autogen.sh

```

notice  that  `autogen.sh`  will   run  the  programs  `autoreconf`  and
`libtoolize`; the  latter is  selected through the  environment variable
`LIBTOOLIZE`,  whose  value  can  be  customised;  for  example  to  run
`glibtoolize` rather than `libtoolize` we do:

```
$ LIBTOOLIZE=glibtoolize sh autogen.sh
```

After this  the procedure  is the same  as the one  for building  from a
proper release tarball, but we have to enable maintainer mode:

```
$ ../configure --enable-maintainer-mode [options]
$ make
$ make check
$ make install
```

## Usage

Read the documentation.

**IMPORTANT** To use the library we  must enable the POSIX features when
including   the   standard   header   files;  so   either   we   include
`ccsys.h`  as   first  header,   or  we  include   the  following
definitions before including **all** the header files:

```
#define _POSIX_C_SOURCE 200809L
```


## Credits

The  stuff was  written by  Marco Maggi.   If this  package exists  it's
because of the great GNU software tools that he uses all the time.


## Bugs

Bug  and vulnerability  reports are  appreciated, all  the vulnerability
reports  are  public; register  them  using  the  Issue Tracker  at  the
project's GitHub  site.  For  contributions and  patches please  use the
Pull Requests feature at the project's GitHub site.


## Resources

The latest release of this package can be downloaded from:

[https://bitbucket.org/marcomaggi/ccsys/downloads](https://bitbucket.org/marcomaggi/ccsys/downloads)

development takes place at:

[http://github.com/marcomaggi/ccsys/](http://github.com/marcomaggi/ccsys/)

and as backup at:

[https://bitbucket.org/marcomaggi/ccsys/](https://bitbucket.org/marcomaggi/ccsys/)

the documentation is available online:

[http://marcomaggi.github.io/docs/ccsys.html](http://marcomaggi.github.io/docs/ccsys.html)

the GNU Project software can be found here:

[http://www.gnu.org/](http://www.gnu.org/)


## Badges and static analysis

### Travis CI

Travis CI is  a hosted, distributed continuous  integration service used
to build and test software projects  hosted at GitHub.  We can find this
project's dashboard at:

[https://travis-ci.org/marcomaggi/ccsys](https://travis-ci.org/marcomaggi/ccsys)

Usage of this  service is configured through the  file `.travis.yml` and
additional scripts are under the directory `meta/travis-ci`.


### Coverity Scan

Coverity Scan is  a service providing the results of  static analysis on
open source coding projects.  We can find this project's dashboard at:

[https://scan.coverity.com/projects/marcomaggi-ccsys](https://scan.coverity.com/projects/marcomaggi-ccsys)

Usage of this  service is implemented with make rules;  see the relevant
section in the file `Makefile.am`.  To access the service a unique token
is needed: this token is private and is owned by the current maintainer.


### Clang's Static Analyzer

The Clang Static Analyzer is a source code analysis tool that finds bugs
in C, C++, and Objective-C programs.  It is distributed along with Clang
and we can find it at:

[http://clang-analyzer.llvm.org/](http://clang-analyzer.llvm.org/)

Usage of this  service is implemented with make rules;  see the relevant
section in the file `Makefile.am`.


### Codecov

Codecov is a service providing code  coverage reports.  We can find this
project's dashboard at:

[https://codecov.io/gh/marcomaggi/ccsys](https://codecov.io/gh/marcomaggi/ccsys)

Usage of  this service is  implemented through direct  interface between
GitHub and Codecov  sites; it configured through  the file `codecov.yml`
and appropriate entries in Travis CI's matrix of builds.

