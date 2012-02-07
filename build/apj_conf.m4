dnl -------------------------------------------------------- -*- autoconf -*-
dnl Licensed to the Apache Software Foundation (ASF) under one or more
dnl contributor license agreements.  See the NOTICE file distributed with
dnl this work for additional information regarding copyright ownership.
dnl The ASF licenses this file to You under the Apache License, Version 2.0
dnl (the "License"); you may not use this file except in compliance with
dnl the License.  You may obtain a copy of the License at
dnl
dnl     http://www.apache.org/licenses/LICENSE-2.0
dnl
dnl Unless required by applicable law or agreed to in writing, software
dnl distributed under the License is distributed on an "AS IS" BASIS,
dnl WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
dnl See the License for the specific language governing permissions and
dnl limitations under the License.


dnl
dnl custom autoconf rules for APRUTIL
dnl

dnl
dnl APJ_FIND_APR: figure out where APR is located
dnl
AC_DEFUN([APJ_FIND_APR], [

  dnl use the find_apr.m4 script to locate APR. sets apr_found and apr_config
  APR_FIND_APR(,,,[1])
  if test "$apr_found" = "no"; then
    AC_MSG_ERROR(APR could not be located. Please use the --with-apr option.)
  fi

  APR_BUILD_DIR="`$apr_config --installbuilddir`"

  dnl make APR_BUILD_DIR an absolute directory (we'll need it in the
  dnl sub-projects in some cases)
  APR_BUILD_DIR="`cd $APR_BUILD_DIR && pwd`"

  APR_INCLUDES="`$apr_config --includes`"
  APR_LIBS="`$apr_config --link-libtool --libs`"
  APR_SO_EXT="`$apr_config --apr-so-ext`"
  APR_LIB_TARGET="`$apr_config --apr-lib-target`"
  APR_MAJOR_VERSION="`$apr_config --version | sed -e 's/^\([[^.]]*\)\..*$/\1/'`"

  AC_SUBST(APR_INCLUDES)
  AC_SUBST(APR_LIBS)
  AC_SUBST(APR_BUILD_DIR)
  AC_SUBST(APR_MAJOR_VERSION)
])

dnl
dnl APJ_FIND_APU: figure out where APR-Util is located
dnl
AC_DEFUN([APJ_FIND_APU], [

  dnl use the find_apr.m4 script to locate APR. sets apr_found and apr_config
  APR_FIND_APU(,,,[1])
  if test "$apu_found" = "no"; then
    AC_MSG_ERROR(APR-Util could not be located. Please use the --with-apr-util option.)
  fi

  APU_BUILD_DIR="`$apu_config --installbuilddir`"

  dnl make APR_BUILD_DIR an absolute directory (we'll need it in the
  dnl sub-projects in some cases)
  APU_BUILD_DIR="`cd $APR_BUILD_DIR && pwd`"

  APU_INCLUDES="`$apu_config --includes`"
  APU_LIBS="`$apu_config --link-libtool --libs`"
  APU_SO_EXT="`$apu_config --apr-so-ext`"
  APU_LIB_TARGET="`$apu_config --apr-lib-target`"
  APU_MAJOR_VERSION="`$apr_config --version | sed -e 's/^\([[^.]]*\)\..*$/\1/'`"

  AC_SUBST(APU_INCLUDES)
  AC_SUBST(APU_LIBS)
  AC_SUBST(APU_BUILD_DIR)
  AC_SUBST(APU_MAJOR_VERSION)
])


