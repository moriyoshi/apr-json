/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef APJ_VERSION_H
#define APJ_VERSION_H

/**
 * @file apj_version.h
 * @brief APR-util Versioning Interface
 * 
 * APR-util's Version
 *
 * There are several different mechanisms for accessing the version. There
 * is a string form, and a set of numbers; in addition, there are constants
 * which can be compiled into your application, and you can query the library
 * being used for its actual version.
 *
 * Note that it is possible for an application to detect that it has been
 * compiled against a different version of APJ by use of the compile-time
 * constants and the use of the run-time query function.
 *
 * APJ version numbering follows the guidelines specified in:
 *
 *     http://apr.apache.org/versioning.html
 */


/* The numeric compile-time version constants. These constants are the
 * authoritative version numbers for APJ. 
 */

/** major version 
 * Major API changes that could cause compatibility problems for older
 * programs such as structure size changes.  No binary compatibility is
 * possible across a change in the major version.
 */
#define APJ_MAJOR_VERSION       0

/** minor version
 * Minor API changes that do not cause binary compatibility problems.
 * Reset to 0 when upgrading APJ_MAJOR_VERSION
 */
#define APJ_MINOR_VERSION       0

/** patch level 
 * The Patch Level never includes API changes, simply bug fixes.
 * Reset to 0 when upgrading APR_MINOR_VERSION
 */
#define APJ_PATCH_VERSION       0

/** 
 * The symbol APJ_IS_DEV_VERSION is only defined for internal,
 * "development" copies of APJ.  It is undefined for released versions
 * of APJ.
 */
/* #define APJ_IS_DEV_VERSION */


#if defined(APJ_IS_DEV_VERSION) || defined(DOXYGEN)
/** Internal: string form of the "is dev" flag */
#define APJ_IS_DEV_STRING "-dev"
#else
#define APJ_IS_DEV_STRING ""
#endif


#ifndef APJ_STRINGIFY
/** Properly quote a value as a string in the C preprocessor */
#define APJ_STRINGIFY(n) APJ_STRINGIFY_HELPER(n)
/** Helper macro for APJ_STRINGIFY */
#define APJ_STRINGIFY_HELPER(n) #n
#endif

/** The formatted string of APJ's version */
#define APJ_VERSION_STRING \
     APJ_STRINGIFY(APJ_MAJOR_VERSION) "." \
     APJ_STRINGIFY(APJ_MINOR_VERSION) "." \
     APJ_STRINGIFY(APJ_PATCH_VERSION) \
     APJ_IS_DEV_STRING

/** An alternative formatted string of APR's version */
/* macro for Win32 .rc files using numeric csv representation */
#define APJ_VERSION_STRING_CSV APJ_MAJOR_VERSION ##, \
                             ##APJ_MINOR_VERSION ##, \
                             ##APJ_PATCH_VERSION


#ifndef APJ_VERSION_ONLY

/* The C language API to access the version at run time, 
 * as opposed to compile time.  APJ_VERSION_ONLY may be defined 
 * externally when preprocessing apr_version.h to obtain strictly 
 * the C Preprocessor macro declarations.
 */

#include "apr_version.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Return APR-util's version information information in a numeric form.
 *
 *  @param pvsn Pointer to a version structure for returning the version
 *              information.
 */
APJ_DECLARE(void) apj_version(apr_version_t *pvsn);

/** Return APJ's version information as a string. */
APJ_DECLARE(const char *) apj_version_string(void);

#ifdef __cplusplus
}
#endif

#endif /* ndef APJ_VERSION_ONLY */

#endif /* ndef APJ_VERSION_H */
