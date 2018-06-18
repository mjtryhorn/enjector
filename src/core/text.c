/*
* text.c
*
* Copyright 2004-2018 Enjector Software, Ltd.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <enjector/core/text.h>
#include <enjector/core/limits.h>
#include <enjector/core/xmemory.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#ifndef _MSC_VER

size_t strncpy_s(char *d, size_t n, char const *s, size_t l)
{
  //strncpy(d, s, n);
    return snprintf(d, n, "%s", s);
}

size_t strncat_s(char *d, char const *s, size_t n)
{
  return snprintf(d, n, "%s%s", d, s);
}

size_t strcat_s(char *d, size_t dn, char const *s)
{
  return snprintf(d, dn, "%s%s", d, s);
}



#endif

void text_free(char* s) {
    assert(s);
    xmemory_free(s);
}

int text_compare_nocase(const char* p1, const char* p2) {
    assert(p1);
    assert(p2);

    unsigned char* s1 = (unsigned char*)p1;
    unsigned char* s2 = (unsigned char*)p2;
    unsigned char c1, c2;

    do {
        c1 = (unsigned char)tolower((int)*s1++);
        c2 = (unsigned char)tolower((int)*s2++);

        // Reached the end?
        if(c1 == 0 && c2 == 0) {
            return 1;
        }

        // Looks like one string is longer than the other
        else if(c1 == 0 || c2 == 0) {
            return c1 - c2;
        }
    } while(c1 == c2);

    return c1 - c2;
}

char* _text_clone(const char* str, const char* filename, size_t line) {
    assert(str);

    return _xmemory_strdup(str, filename, line);
}

char* _text_clone_length(const char* str, size_t str_len, const char* filename, size_t line) {
    assert(str);

    char* str_copy = (char*)_xmemory_malloc(str_len + 1, filename, line);

    memcpy(str_copy, str, str_len);
    str_copy[str_len] = 0;

    return str_copy;
}

char* text_copy(char* dest, const char* src) {
    assert(dest);
    assert(src);

    char* d = dest;
    char c;

    if(src == NULL) {
        *d = '\0';
    } else {
        do {
            c = *src++;
            *d++ = c;
        } while('\0' != c);
    }

    return dest;
}

char* text_copy_length(char* dest, const char* src, size_t len) {
    assert(dest);
    assert(src);

    char* d = dest;

    while(len-- > 0) {
        char c = *src++;
        *d++ = c;
    }

    return dest;
}

size_t text_length(const char* str) {
    assert(str);

    const char* ptr = str;

    while(*str++) {}

    return str - ptr - 1;
}

int text_compare(const char* s1, const char* s2) {
    assert(s1);
    assert(s2);

    while(*s1 == *s2) {
        if(*s1 == '\0')
            return(0);

        ++s1;
        ++s2;
    }

    return(*s1 - *s2);
}

void text_replace_inplace(char* source, char old_value, char new_value) {
    assert(source);

    for(char* p = source; *p; p++) {
        if(*p == old_value) {
            *p = new_value;
        }
    }
}

char* _text_replace(const char* source, const char* old_value, const char* new_value, const char* filename, size_t line) {
    assert(source);
    assert(old_value);
    assert(new_value);

    if(old_value == NULL || new_value == NULL) {
        return _text_clone(source, filename, line);
    }

    // Increment positions cache size initially by this number.
    size_t cache_sz_inc = 16;

    // Thereafter, each time capacity needs to be increased,
    // multiply the increment by this factor.
    const size_t cache_sz_inc_factor = 3;

    // But never increment capacity by more than this number.
    const size_t cache_sz_inc_max = 1048576;

    char* ret = NULL;
    const char* pstr2, *pstr = source;
    size_t count = 0;
    size_t* pos_cache = NULL;
    size_t cache_sz = 0;
    size_t retlen, newlen = 0, oldlen = strlen(old_value);

    // Find all matches and cache their positions.
    while((pstr2 = strstr(pstr, old_value)) != NULL) {
        count++;

        // Increase the cache size when necessary.
        if(cache_sz < count) {
            cache_sz += cache_sz_inc;
            pos_cache = realloc(pos_cache, sizeof(*pos_cache) * cache_sz);

            if(pos_cache == NULL) {
                goto end_repl_str;
            }

            cache_sz_inc *= cache_sz_inc_factor;

            if(cache_sz_inc > cache_sz_inc_max) {
                cache_sz_inc = cache_sz_inc_max;
            }
        }

        pos_cache[count - 1] = pstr2 - source;
        pstr = pstr2 + oldlen;
    }

    size_t orglen = pstr - source + strlen(pstr);

    // Allocate memory for the post-replacement string.
    if(count > 0) {
        newlen = text_length(new_value);
        retlen = orglen + (newlen - oldlen) * count;
    } else {
        retlen = orglen;
    }

    ret = _xmemory_malloc(retlen + 1, filename, line);

    if(ret == NULL) {
        goto end_repl_str;
    }

    if(count == 0) {
        // If no matches, then just duplicate the string.
        text_copy(ret, source);
    } else {
        // Otherwise, duplicate the string whilst performing
        // the replacements using the position cache.
        char* pret = ret;
        memcpy(pret, source, pos_cache[0]);
        pret += pos_cache[0];

        for(size_t i = 0; i < count; i++) {
            memcpy(pret, new_value, newlen);
            pret += newlen;
            pstr = source + pos_cache[i] + oldlen;
            size_t cpylen = (i == count - 1 ? orglen : pos_cache[i + 1]) - pos_cache[i] - oldlen;
            memcpy(pret, pstr, cpylen);
            pret += cpylen;
        }

        ret[retlen] = '\0';
    }

end_repl_str:

    // Free the cache and return the post-replacement string,
    // which will be NULL in the event of an error.
    free(pos_cache);
    return ret;
}

char* _text_trim(const char* s, const char* filename, size_t line) {
    assert(s);

    size_t p_left = 0;
    size_t p_right = 0;
    const size_t n = text_length(s);

    char c;

    size_t index = 0;

    while(c = s[index++], index < n && (c == ' ' || c == '\r' || c == '\n')) {
        p_left = index;
    }

    index = n;

    while(c = s[--index], index > 0 && (c == ' ' || c == '\r' || c == '\n')) {
        p_right = index;
    }

    if(p_left > p_right) return NULL;

    // Nothing to trim?
    if(p_right == 0 || p_left == n - 1) return _text_clone(s, filename, line);

    const size_t rs = p_right - p_left;
    char* r = _xmemory_malloc(rs + 1, filename, line);
    text_copy_length(r, &s[p_left], rs);

    return r;
}

char* _text_trim_left(const char* s, const char* filename, size_t line) {
    assert(s);

    size_t p = 0;
    const size_t n = text_length(s);
    char c;

    size_t index = 0;

    while(c = s[index++], index < n && (c == ' ' || c == '\r' || c == '\n')) {
        p = index;
    }

    // Nothing to trim?
    if(p == n - 1) return _text_clone(s, filename, line);

    size_t rs = n - p;
    char* r = _xmemory_malloc(rs + 1, filename, line);
    text_copy_length(r, &s[p], rs);

    return r;
}

char* _text_trim_right(const char* s, const char* filename, size_t line) {
    assert(s);

    size_t p = 0;
    char c;

    size_t index = strlen(s);

    while(c = s[--index], index > 0 && (c == ' ' || c == '\r' || c == '\n')) {
        p = index;
    }

    // Nothing to trim?
    if(p == 0) return _text_clone(s, filename, line);

    const size_t rs = p;
    char* r = _xmemory_malloc(p + 1, filename, line);
    text_copy_length(r, s, p);
    r[rs] = 0;

    return r;
}

bool text_is_null_or_empty(const char* s) {
    if(s == NULL) {
        return true;
    }

    if(strlen(s) == 0) {
        return true;
    }

    return false;
}

char* _text_to_lower(const char* s, const char* filename, size_t line) {
    assert(s);

    size_t n = text_length(s);
    char* r = _xmemory_malloc(n, filename, line);

    while(n-- > 0) {
        r[n] = tolower(s[n]);
    }

    return r;
}

char* _text_to_upper(const char* s, const char* filename, size_t line) {
    assert(s);

    size_t n = text_length(s);
    char* r = _xmemory_malloc(n, filename, line);

    while(n-- > 0) {
        r[n] = toupper(s[n]);
    }

    return r;
}

const char* text_last_index_of(const char* s, char c) {
    assert(s);

    size_t len = (int)strlen(s);
    char* p = ((char*)s) + len;

    while(--p > s) {
        if(*p == c) {
            return p;
        }
    }

    return NULL;
}

bool text_equals(const char* s1, const char* s2) {
    assert(s1);
    assert(s2);

    return strcmp(s1, s2) == 0;
}

bool text_starts_with(const char* s1, const char* s2) {
    assert(s1);
    assert(s2);

    return strstr(s1, s2) == s1;
}

char* text_append(char* target, size_t target_size, const char* source) {
    assert(target);
    assert(source);

    strcat_s(target, target_size, source);
    return target;
}

char* _text_format(const char* filename, size_t line, const char* format, ...) {
    assert(format);

    va_list args;
    va_start(args, format);

    char message[MAX_LENGTH_TEXT_FORMAT_BUFFER];
    vsnprintf(message, MAX_LENGTH_TEXT_FORMAT_BUFFER, format, args);

    va_end(args);

    return _text_clone(message, filename, line);
}

list* _text_split(const char* str, char split_char, const char* filename, size_t line) {
    assert(str);

    list* rows = _list_create(filename, line);

    char c;
    char* start = (char*)str;

    do {
        while(c = *str++, c != 0 && c != split_char) {}

        char* row = _text_clone_length(start, str - start - 1, filename, line);
        list_add(rows, row);
        start = (char*)str;
    } while(c != 0);

    return rows;
}
