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

#include "apr_json.h"

typedef struct apr_json_serializer_t {
    apr_pool_t *pool;
    apr_bucket_brigade *brigade;
} apr_json_serializer_t;

static void apr_json_encode_value(apr_json_serializer_t *self, const apr_json_value_t *value);

static void brigade_append_immortal(apr_bucket_brigade *brigade, const char *buf, apr_size_t nbyte)
{
    APR_BRIGADE_INSERT_TAIL(brigade, apr_bucket_immortal_create(buf, nbyte, brigade->bucket_alloc));
}

static void brigade_append_transient(apr_bucket_brigade *brigade, const char *buf, apr_size_t nbyte)
{
    APR_BRIGADE_INSERT_TAIL(brigade, apr_bucket_transient_create(buf, nbyte, brigade->bucket_alloc));
}

static void apr_json_encode_string(apr_json_serializer_t *self, const apr_json_string_t *string)
{
    const char *p, *e, *chunk;
    brigade_append_immortal(self->brigade, "\"", 1);
    for (p = chunk = string->p, e = string->p + string->len; p < e; p++) {
        switch(*p) {
        case '\n':
            brigade_append_transient(self->brigade, chunk, p - chunk);
            brigade_append_immortal(self->brigade, "\\n", 2);
            chunk = p + 1;
            break;
        case '\r':
            brigade_append_transient(self->brigade, chunk, p - chunk);
            brigade_append_immortal(self->brigade, "\\r", 2);
            chunk = p + 1;
            break;
        case '\t':
            brigade_append_transient(self->brigade, chunk, p - chunk);
            brigade_append_immortal(self->brigade, "\\t", 2);
            chunk = p + 1;
            break;
        case '\b':
            brigade_append_transient(self->brigade, chunk, p - chunk);
            brigade_append_immortal(self->brigade, "\\b", 2);
            chunk = p + 1;
            break;
        case '\f':
            brigade_append_transient(self->brigade, chunk, p - chunk);
            brigade_append_immortal(self->brigade, "\\f", 2);
            chunk = p + 1;
            break;
        case '\\':
            brigade_append_transient(self->brigade, chunk, p - chunk);
            brigade_append_immortal(self->brigade, "\\\\", 2);
            chunk = p + 1;
            break;
        case '"':
            brigade_append_transient(self->brigade, chunk, p - chunk);
            brigade_append_immortal(self->brigade, "\\\"", 2);
            chunk = p + 1;
            break;
        default:
            break;
        }
    }
    if (chunk < p)
        brigade_append_transient(self->brigade, chunk, p - chunk);
    brigade_append_immortal(self->brigade, "\"", 1);
}


static apr_status_t apr_json_encode_array(apr_json_serializer_t *self, apr_array_header_t *array)
{
    brigade_append_immortal(self->brigade, "[", 1);
    apr_size_t i;
    for (i = 0; i < array->nelts; i++) { 
        if (i > 0)
            brigade_append_immortal(self->brigade, ", ", 2);
        apr_json_encode_value(self, ((apr_json_value_t **)array->elts)[i]);
    }
    brigade_append_immortal(self->brigade, "]", 1);
}

static void apr_json_encode_object(apr_json_serializer_t *self, apr_hash_t *object)
{
    apr_hash_index_t *i;
    int first = 1;
    brigade_append_immortal(self->brigade, "{", 1);

    for (i = apr_hash_first(self->pool, object); i; i = apr_hash_next(i)) {
        const char *key;
        apr_ssize_t key_len;
        apr_json_value_t *value;
        apr_hash_this(i, (const void **)&key, &key_len, (void **)&value);

        if (!first)
            brigade_append_immortal(self->brigade, ",", 1);
        {
            apr_json_string_t key_str = { key, key_len };
            apr_json_encode_string(self, &key_str);
            brigade_append_immortal(self->brigade, ":", 1);
            apr_json_encode_value(self, value);
        }
        first = 0;
    }
    brigade_append_immortal(self->brigade, "}", 1);
}

static void apr_json_encode_value(apr_json_serializer_t *self, const apr_json_value_t *value)
{ 
    switch(value->type){
    case APR_JSON_STRING:
        apr_json_encode_string(self, &value->value.string);
        break;
    case APR_JSON_LONG: 
        apr_brigade_printf(self->brigade, NULL, NULL, "%ld", value->value.lnumber);
        break;
    case APR_JSON_DOUBLE:
        apr_brigade_printf(self->brigade, NULL, NULL, "%lf", value->value.dnumber);
        break;
    case APR_JSON_BOOLEAN:
        brigade_append_immortal(self->brigade, value->value.boolean ? "true": "false", 4);
        break;
    case APR_JSON_NULL:
        brigade_append_immortal(self->brigade, "null", 4);
        break;
    case APR_JSON_OBJECT:
        apr_json_encode_object(self, value->value.object);
        break;
    case APR_JSON_ARRAY:
        apr_json_encode_array(self, value->value.array);
        break;
    }
}

void apr_json_encode(apr_bucket_brigade *brigade, const apr_json_value_t *json, apr_pool_t *pool)
{
    apr_json_serializer_t serializer = { pool, brigade };
    apr_json_encode_value(&serializer, json);
}
