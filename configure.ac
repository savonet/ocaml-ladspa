# check for one particular file of the sources 
AC_INIT([ocaml-ladspa],[0.1.5],[savonet-users@lists.sourceforge.net])

VERSION=$PACKAGE_VERSION
AC_MSG_RESULT([configuring $PACKAGE_STRING])

AC_BASE_CHECKS()

# LADSPA check
AC_CHECK_HEADER(ladspa.h, [], [AC_MSG_ERROR([Cannot find ladspa.h, please 
install the LADSPA_SDK, or put ladspa.h somewhere it can be found]);])

# substitutions to perform
AC_SUBST(VERSION)

case "$host" in
   *-mingw*) LIBS="$LIBS dl";
esac

# Finally create the Makefile and samples
AC_CONFIG_FILES([Makefile],[chmod a-w Makefile])
AC_CONFIG_FILES([src/META])
AC_CONFIG_FILES([src/Makefile])
AC_OUTPUT
