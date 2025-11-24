AC_PREREQ([2.71])
AC_INIT([growable-buf], [0.0.1])
AM_INIT_AUTOMAKE([foreign])
LT_INIT([disable-static])
AC_CONFIG_SRCDIR([src])
AC_PROG_CC
PKG_CHECK_MODULES([CHECK],[check])
AC_ARG_ENABLE([gcov],
    [AS_HELP_STRING([--enable-gcov], [use Gcov])],
    [], [enable_gcov=no])
AM_CONDITIONAL([COND_GCOV],[test "$enable_gcov" = yes])
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT
