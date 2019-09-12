Overview
========
This repository exists as a staging ground for developing candidate contributions to the 
GNU Compiler Collection ([GCC]).  Fortran 2018 features that started in this repository 
and have since been released in GCC include

* Teams 
* `ISO_Fortran_binding.h`

Current candidate features in this site that have not yet been committed to the GCC repository
include
- [ ] `co_broadcast` of derived types. 

We welcome additional code contributions via [pull request] or donations to [Sourcery Institute]
to support additional Fortran 2018 feature support.

Installation
============
We recommend building the [latest release] using the [OpenCoarrays] `install.sh` script as follows:
```bash
git clone https://github.com/sourceryinstitute/opencoarrays
cd oppencoarrays
./install.sh --package gcc \
  --from-url https://github.com/sourceryinstitute/gcc/archive/<release-tag>.tar.gz \
  --install-version <release-tag> \
  --num-threads 4
```
where `<release-tag>` must be replaced with the appropriate string such as co_broadcast-20190825.
If it fails, retry with `--bootstrap`, which takes _much_ longer but has a higher chance of working
when building with older versions of GCC.  Also, for a non-interactive build, add `--yes-to-all` to
instruct the installer to assume affirmative answers to any queries. If successful, the above steps 
will install GCC in the `prerequisites/installations` subdirectory.  To see additional installation
options, including choosing another installation path, execute `./install.sh --help`.

Next use the resulting compilers to build [MPICH] 3.2 or later:
```bash

# Install MPICH 3.2 or later using the GCC that was installed above:
export LD_LIBRARY_PATH="${PWD}"/prerequisites/installations/gcc/teams-<release-tag>/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH="${PWD}"/prerequisites/installations/gcc/teams-<release-tag>/lib64:$LD_LIBRARY_PATH

./install.sh --package mpich \
   --with-fortran "${PWD}"/prerequisites/installations/gcc/<release-tag>/bin/gfortarn \
   --with-c       "${PWD}"/prerequisites/installations/gcc/<release-tag>/bin/gcc \
   --with-cpp     "${PWD}"/prerequisites/installations/gcc/<release-tag>bin/g++ \
   --num-threads 4
```
 the [opencoarrays-teams branch] of OpenCoarrays.  
```bash
# Install OpenCoarrays using the MPICH just installed:
./install.sh  \
   --with-mpi "${PWD}"/prerequisites/installations/mpich/<mpich-version-number>/
```
after replacing `<mpich-version-number>` with... wait for it... the MPI version number (3.2 or later). :) 
Please report any problems with the above steps on our [issues page].


[GCC mirror] README
===================

This directory contains the GNU Compiler Collection (GCC).

The GNU Compiler Collection is free software.  See the files whose
names start with COPYING for copying permission.  The manuals, and
some of the runtime libraries, are under different terms; see the
individual source files for details.

The directory INSTALL contains copies of the installation information
as HTML and plain text.  The source of this information is
gcc/doc/install.texi.  The installation information includes details
of what is included in the GCC sources and what files GCC installs.

See the file gcc/doc/gcc.texi (together with other files that it
includes) for usage and porting information.  An online readable
version of the manual is in the files gcc/doc/gcc.info*.

See http://gcc.gnu.org/bugs/ for how to report bugs usefully.

Copyright years on GCC source files may be listed using range
notation, e.g., 1987-2012, indicating that every year in the range,
inclusive, is a copyrightable year that could otherwise be listed
individually.

[GCC mirror]: https://github.com/gcc-mirror/gcc
[GCC]: https://gcc.gnu.org/gcc
[OpenCoarrays]: https://www.opendcoarrays.org
[MPICH]: https://www.mpich.org
[teams branch]: https://github.com/sourceryinstitute/gcc/tree/teams
[issue-#6-iso-fortran-binding-h branch]: https://github.com/sourceryinstitute/gcc/tree/issue-#6-iso-fortran-binding-h
[download-opencoarrays-mpich branch]: https://github.com/sourceryinstitute/gcc/tree/download-opencoarrays-mpich 
[releases]: https://github.com/sourceryinstitute/gcc/releases/
[issues page]: https://github.com/sourceryinstitute/gcc/issues/
[opencoarrays-teams branch]: https://github.com/sourceryinstitute/opencoarrays/tree/opencoarrays-teams
[pull request]: https://github.com/sourceryinstitute/gcc/pulls
[latest release]: https://github.com/sourceryinstitute/gcc/releases/latest
[Sourcery Institute]: https://www.sourceryinstitute.org
