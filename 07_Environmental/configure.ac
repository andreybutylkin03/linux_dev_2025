AC_INIT([rhasher], [1.0])
AC_CONFIG_SRCDIR([src/rhasher.c])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CC

PKG_CHECK_MODULES([RHASH], [librhash])

AC_ARG_ENABLE([readline],
    [AS_HELP_STRING([--disable-readline], [Disable readline support])],
    [enable_readline=$enableval],
    [enable_readline=yes])

AS_IF([test "x$enable_readline" != "xno"], [
    AC_CHECK_LIB([readline], [readline], [
        AC_CHECK_HEADER([readline/readline.h], [
            AC_DEFINE([HAVE_READLINE], [1], [readline support])
            READLINE_LIBS="-lreadline"
        ])
    ])
])

AC_SUBST([READLINE_LIBS])
AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
