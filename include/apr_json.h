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

#ifndef APR_JSON_H
#define APR_JSON_H

/**
 * @file apr_json.h
 * @brief APR-JSON routines
 */

#include "apr.h"
#include "apr_pools.h"
#include "apr_tables.h"
#include "apr_hash.h"
#include "apr_strings.h"
#include "apr_buckets.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup APR_JSON JSON functions
 * @ingroup APR_JSON
 * @{
 */

/**
 * APJ_DECLARE_EXPORT is defined when building the APR-UTIL dynamic library,
 * so that all public symbols are exported.
 *
 * APJ_DECLARE_STATIC is defined when including the APR-UTIL public headers,
 * to provide static linkage when the dynamic library may be unavailable.
 *
 * APJ_DECLARE_STATIC and APJ_DECLARE_EXPORT are left undefined when
 * including the APR-UTIL public headers, to import and link the symbols from 
 * the dynamic APR-UTIL library and assure appropriate indirection and calling
 * conventions at compile time.
 */

/**
 * The public APR-UTIL functions are declared with APJ_DECLARE(), so they may
 * use the most appropriate calling convention.  Public APR functions with 
 * variable arguments must use APJ_DECLARE_NONSTD().
 *
 * @fn APJ_DECLARE(rettype) apr_func(args);
 */
#define APJ_DECLARE(type)            type
/**
 * The public APR-UTIL functions using variable arguments are declared with 
 * APJ_DECLARE_NONSTD(), as they must use the C language calling convention.
 *
 * @fn APJ_DECLARE_NONSTD(rettype) apr_func(args, ...);
 */
#define APJ_DECLARE_NONSTD(type)     type
/**
 * The public APR-UTIL variables are declared with APJ_DECLARE_DATA.
 * This assures the appropriate indirection is invoked at compile time.
 *
 * @fn APJ_DECLARE_DATA type apr_variable;
 * @note APJ_DECLARE_DATA extern type apr_variable; syntax is required for
 * declarations within headers to properly import the variable.
 */
#define APJ_DECLARE_DATA

#if !defined(WIN32) || defined(APJ_MODULE_DECLARE_STATIC)
/**
 * Declare a dso module's exported module structure as APJ_MODULE_DECLARE_DATA.
 *
 * Unless APJ_MODULE_DECLARE_STATIC is defined at compile time, symbols 
 * declared with APJ_MODULE_DECLARE_DATA are always exported.
 * @code
 * module APJ_MODULE_DECLARE_DATA mod_tag
 * @endcode
 */
#define APJ_MODULE_DECLARE_DATA
#else
#define APJ_MODULE_DECLARE_DATA           __declspec(dllexport)
#endif

/**
 * Enum that represents the type of the given JSON value.
 */
typedef enum apr_json_type_e { 
	APR_JSON_OBJECT,
	APR_JSON_ARRAY,
	APR_JSON_STRING,
	APR_JSON_LONG,
	APR_JSON_DOUBLE,
	APR_JSON_BOOLEAN,
	APR_JSON_NULL
} apr_json_type_e;

/** 
 * A structure to hold a JSON string.
 */
typedef struct apr_json_string_t {
	const char *p;
    apr_size_t len;
} apr_json_string_t;

/**
 * A structure that holds a JSON value.
 */
typedef struct apr_json_value_t {
	apr_json_type_e type;
	union {
		apr_hash_t *object;
		apr_array_header_t *array;
		double dnumber;
		long lnumber;
		apr_json_string_t string;
		int boolean;
	} value;
} apr_json_value_t;

APJ_DECLARE(apr_status_t) apr_json_decode(apr_json_value_t **retval, const char *injson, apr_size_t size, apr_pool_t *pool);
 
APJ_DECLARE(void) apr_json_encode(apr_bucket_brigade *brigade, const apr_json_value_t *json, apr_pool_t *pool);

/** @} */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !APR_JSON_H */
