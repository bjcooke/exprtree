AC_INIT([exprtree], [0.1], [bjcooke92@gmail.com])
dnl
AC_CONFIG_SRCDIR([src/])
dnl
AC_PROG_CC
dnl
AC_PATH_PROG(AWK,[awk])
AC_PATH_PROG(PRINTF,[printf])
AC_PATH_PROG(RM,[rm])
AC_PATH_PROG(SED,[sed])
AC_PATH_PROG(pkgconf,[pkg-config])
dnl
AC_HEADER_STDC
AC_CHECK_HEADERS([getopt.h stdbool.h term.h])
dnl
[

if [ -f /usr/lib/libtermcap.a -o -f /lib/libtermcap.a ]; then
	tlib=-ltermcap
else
	tlib=-ltinfo
fi

CFLAGS="$CFLAGS -Wall -pedantic --std=c99"
LDFLAGS="$LDFLAGS -Wall -pedantic"
LIBS="$LIBS $tlib"
]
dnl
AC_CONFIG_FILES([
	Makefile
	script/match-header.awk
	script/match-header.sed
	script/print_token_id-source.awk
	script/scan_tokens-source.awk
	script/scan_tokens-source.sed
	script/token-header.awk
	script/tokenid-header.awk
])
dnl
AC_OUTPUT
