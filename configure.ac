AC_INIT([bjcalc], [0.1], [bcooke@fofk.org])
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
AC_PATH_PROG(pkgconf,[pkgconf])
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

for gpl_lib in libgmp libmpfr; do
	for dir in $lib_path; do
		if [ -d "$dir" ]; then
			if $ac_cv_path_find $dir -maxdepth 1 -type f -name ${gpl_lib}.so -print -quit | $ac_cv_path_SED -n '/./!q1'
			then
				LIBS="$LIBS $dir/${gpl_lib}.so"
				$ac_cv_path_PRINTF 'configure: %s found.\n' "${gpl_lib}.so"
				break
			fi
		fi
	done
done

LIBS="$LIBS `$ac_cv_path_pkgconf --libs tinfo`"
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
