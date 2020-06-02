AC_INIT([exprtree], [0.1], [bcooke@fofk.org])
dnl
AC_CONFIG_SRCDIR([src/])
dnl
AC_PROG_CC
dnl
AC_PATH_PROG(AWK,[awk])
AC_PATH_PROG(MKDIR,[mkdir])
AC_PATH_PROG(PRINTF,[printf])
AC_PATH_PROG(RM,[rm])
AC_PATH_PROG(SED,[sed])
AC_PATH_PROG(find,[find])
AC_PATH_PROG(pkgconf,[pkg-config])
dnl
AC_HEADER_STDC
AC_CHECK_HEADERS([stdbool.h gmp.h mpfr.h term.h])
dnl
[
lib_path=\
`
	$CC -Xlinker --verbose 2>&1 |
	$ac_cv_path_SED -n 's/SEARCH_DIR("=\([A-Za-z0-9_/-]\+\)");/\1/gp'
`

LIBS="$LIBS -lgmp -lmpfr `$ac_cv_path_pkgconf --libs tinfo`"
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
