# Process this with autoconf to produce a configure script:
# aclocal && autoconf && autoheader && automake

AC_PREREQ(2.69)

AC_INIT([ISO Fortran Binding Library for C-Fortran Interoperability], 1, [fortran@gcc.gnu.org, gcc@gcc.gnu.org])

AC_PROG_CC

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_CONFIG_HEADER(config.h)

AC_CANONICAL_SYSTEM
ACX_NONCANONICAL_TARGET

target_alias=${target_alias-$host_alias}
AC_SUBST(target_alias)

toolexecdir='$(libdir)/gcc-lib/$(target_alias)'
toolexeclibdir='$(libdir)'
AC_SUBST(toolexecdir)
AC_SUBST(toolexeclibdir)

GCC_BASE_VER

AC_CONFIG_FILES(Makefile)

AC_OUTPUT
