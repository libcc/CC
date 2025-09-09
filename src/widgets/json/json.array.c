/*
 * Copyright libcc.cn@gmail.com. and other libcc contributors.
 * All rights reserved.org>
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:

 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/
#include "json.c.h"

#define _JSON_ARRAY_SIZE_ 64


void _json_array_alloc(_cc_json_t* ctx, size_t capacity) {
    ctx->type = _CC_JSON_ARRAY_;
    ctx->element.uni_array = _cc_alloc_array(capacity);
}

void _destroy_json_array(_cc_json_t* ctx) {
    uintptr_t arr = (uintptr_t)ctx->element.uni_array;
    size_t i;
    size_t length = _cc_array_length(arr);

    for (i = 0; i < length; i++) {
        _cc_json_t *v = ((_cc_json_t*)*((uintptr_t*)(arr) + i));
        _json_free_node(v);
    }
    _cc_free_array(arr);
}

/**/
_CC_API_PUBLIC(_cc_json_t*) _cc_json_alloc_array(const tchar_t *keyword, size_t capacity) {
    _cc_json_t *item = (_cc_json_t *)_cc_malloc(sizeof(_cc_json_t));
    bzero(item, sizeof(_cc_json_t));
    item->type = _CC_JSON_ARRAY_;
    item->element.uni_array = _cc_alloc_array(capacity);
    if (keyword) {
        item->name = _cc_sds_alloc(keyword,_tcslen(keyword));
    } else {
        item->name = nullptr;
    }

    return item;
}

_CC_API_PUBLIC(size_t) _json_array_push(_cc_json_t *ctx, _cc_json_t *data) {
    return _cc_array_push(&ctx->element.uni_array, (uintptr_t)data);
}

/**/
_CC_API_PUBLIC(bool_t) _cc_json_array_remove(_cc_json_t *ctx, const uint32_t index) {
    uintptr_t r;
    if (ctx->type != _CC_JSON_ARRAY_) {
        return false;
    }

    r = _cc_array_remove(ctx->element.uni_array, index);
    if (r == -1) {
        return false;
    }

    _json_free_node((_cc_json_t *)(uintptr_t*)r);
    return true;
}

/**/
_CC_API_PUBLIC(bool_t) _cc_json_array_push(_cc_json_t *ctx, _cc_json_t *data) {
    if (ctx->type != _CC_JSON_ARRAY_) {
        return false;
    }
    return _json_array_push(ctx, data) != -1;
}