dnl config.m4 for extension nump

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(nump, for nump support,
dnl Make sure that the comment is aligned:
dnl [  --with-nump             Include nump support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(nump, whether to enable nump support,
dnl Make sure that the comment is aligned:
[  --enable-nump          Enable nump support], no)

if test "$PHP_NUMP" != "no"; then
  dnl Write more examples of tests here...

  dnl # get library FOO build options from pkg-config output
  dnl AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  dnl AC_MSG_CHECKING(for libfoo)
  dnl if test -x "$PKG_CONFIG" && $PKG_CONFIG --exists foo; then
  dnl   if $PKG_CONFIG foo --atleast-version 1.2.3; then
  dnl     LIBFOO_CFLAGS=\`$PKG_CONFIG foo --cflags\`
  dnl     LIBFOO_LIBDIR=\`$PKG_CONFIG foo --libs\`
  dnl     LIBFOO_VERSON=\`$PKG_CONFIG foo --modversion\`
  dnl     AC_MSG_RESULT(from pkgconfig: version $LIBFOO_VERSON)
  dnl   else
  dnl     AC_MSG_ERROR(system libfoo is too old: version 1.2.3 required)
  dnl   fi
  dnl else
  dnl   AC_MSG_ERROR(pkg-config not found)
  dnl fi
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBDIR, NUMP_SHARED_LIBADD)
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)

  dnl # --with-nump -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/nump.h"  # you most likely want to change this
  dnl if test -r $PHP_NUMP/$SEARCH_FOR; then # path given as parameter
  dnl   NUMP_DIR=$PHP_NUMP
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for nump files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       NUMP_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$NUMP_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the nump distribution])
  dnl fi

  dnl # --with-nump -> add include path
  dnl PHP_ADD_INCLUDE($NUMP_DIR/include)

  dnl # --with-nump -> check for lib and symbol presence
  dnl LIBNAME=NUMP # you may want to change this
  dnl LIBSYMBOL=NUMP # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $NUMP_DIR/$PHP_LIBDIR, NUMP_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_NUMPLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong nump lib version or lib not found])
  dnl ],[
  dnl   -L$NUMP_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(NUMP_SHARED_LIBADD)

  dnl # In case of no dependencies
  AC_DEFINE(HAVE_NUMP, 1, [ Have nump support ])

  PHP_NEW_EXTENSION(nump,                     \
                                              \
  src/common.c                                \
  src/nump/mt_base.c                          \
  src/nump/mt_array.c                         \
  src/nump/mt_string.c                        \
  src/nump/mt_linalg.c                        \
  src/nump/mt_random.c                        \
  src/nump/mt_twodim.c                        \
  src/nump/mt_math.c                          \
  src/nump/mt_apply.c                         \
  src/php/objects/php_mt_o.c                  \
  src/php/handlers/php_common_handlers.c      \
  src/php/handlers/php_mt_handlers.c          \
  src/php/classes/php_mt_ce.c                 \
  src/php/functions/php_mt_fn.c               \
                                              \
  php_nump.c                                  \
                                              \
  , $ext_shared)

  PHP_ADD_BUILD_DIR($ext_builddir/src, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/nump, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/php, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/php/objects, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/php/classes, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/php/handlers, 1)
  PHP_ADD_BUILD_DIR($ext_builddir/src/php/functions, 1)

  PHP_ADD_EXTENSION_DEP(nump, spl)
fi
