AC_PREREQ([2.69])
AC_INIT([fusion-reactor], [1.0], [dylan@fusion-core.org])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])


AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AC_PROG_LN_S
AC_PROG_RANLIB
AC_PROG_CC_C_O

#AC_CHECK_LIB([ncurses])

#checking for functions, headers
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([munmap])

AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([stdint.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([unistd.h])

AC_C_INLINE
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_PREREQ
AC_TYPE_INT32_T
AC_TYPE_MODE_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

#check for Fusion Core build tools


AC_CONFIG_FILES([Makefile
				 src/Makefile
				 fusion-elf/Makefile])

AC_OUTPUT
