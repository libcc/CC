#include "json.c.h"
#include <libcc/math.h>

static const tchar_t* char2escape[256] = {
    _T("\\u0000"), _T("\\u0001"), _T("\\u0002"), _T("\\u0003"), _T("\\u0004"), _T("\\u0005"),
    _T("\\u0006"), _T("\\u0007"), _T("\\b"),     _T("\\t"),     _T("\\n"),     _T("\\u000b"),
    _T("\\f"),     _T("\\r"),     _T("\\u000e"), _T("\\u000f"), _T("\\u0010"), _T("\\u0011"),
    _T("\\u0012"), _T("\\u0013"), _T("\\u0014"), _T("\\u0015"), _T("\\u0016"), _T("\\u0017"),
    _T("\\u0018"), _T("\\u0019"), _T("\\u001a"), _T("\\u001b"), _T("\\u001c"), _T("\\u001d"),
    _T("\\u001e"), _T("\\u001f"),       nullptr,          nullptr,          _T("\\\""),       nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          /*_T("\\/")*/ nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          _T("\\\\"),       nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          _T("\\u007f"),    nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,          nullptr,          nullptr,
    nullptr,          nullptr,          nullptr,          nullptr,
};


_CC_API_PUBLIC(_cc_json_t*) _cc_json_array_find(const _cc_json_t *ctx, uint32_t index) {
    _cc_assert(ctx != nullptr);

    if (ctx->type == _CC_JSON_ARRAY_) {
        uintptr_t arr = _cc_array_get(ctx->element.uni_array, index);
        if (arr != -1) {
            return (_cc_json_t*)arr;
        }
    }

    return nullptr;
}

_CC_API_PUBLIC(_cc_json_t*) _cc_json_object_find(const _cc_json_t *ctx, const tchar_t *keyword) {
    _cc_assert(ctx != nullptr);

    if (ctx->type == _CC_JSON_OBJECT_) {
        _cc_rbtree_iterator_t *node = _cc_rbtree_get(&ctx->element.uni_object, (uintptr_t)keyword, _json_get_object);
        if (node) {
            return _cc_upcast(node, _cc_json_t, lnk);
        }
    }

    return nullptr;
}

void _json_free_object_rb_node(_cc_rbtree_iterator_t *node) {
    _json_free_node(_cc_upcast(node, _cc_json_t, lnk));
}

void _json_free_node(_cc_json_t *item) {
    _cc_assert(item != nullptr);
    switch (item->type) {
    case _CC_JSON_OBJECT_:
        _cc_rbtree_destroy(&item->element.uni_object, _json_free_object_rb_node);
        break;
    case _CC_JSON_STRING_:
        if (item->element.uni_string) {
            _cc_sds_free(item->element.uni_string);
        }
        break;
    case _CC_JSON_ARRAY_:
        _destroy_json_array(item);
        break;
    }

    if (item->name) {
        _cc_sds_free(item->name);
    }
    _cc_free(item);
}

void _destroy_json_object(_cc_json_t *root) {
    _cc_rbtree_destroy(&root->element.uni_object, _json_free_object_rb_node);
}

_CC_API_PUBLIC(void) _cc_free_json(_cc_json_t *item) {
    if (_cc_unlikely(item == nullptr)) {
        return;
    }
    _json_free_node(item);
}

/**/
_CC_API_PUBLIC(_cc_json_t*) _cc_json_add_boolean(_cc_json_t *ctx, const tchar_t *keyword, bool_t value) {
    _cc_json_t *item = _cc_json_alloc_object(_CC_JSON_BOOLEAN_, keyword);
    item->element.uni_boolean = value;

    if (ctx->type == _CC_JSON_ARRAY_) {
        _json_array_push(ctx, item);
        return item;
    } else if (ctx->type == _CC_JSON_OBJECT_) {
        _json_object_push(ctx, item, true);
        return item;
    }
    _json_free_node(item);
    return nullptr;
}

/**/
_CC_API_PUBLIC(_cc_json_t*) _cc_json_add_number(_cc_json_t *ctx, const tchar_t *keyword, int64_t value) {
    _cc_json_t *item = _cc_json_alloc_object(_CC_JSON_INT_, keyword);
    item->element.uni_int = value;
    if (ctx->type == _CC_JSON_ARRAY_) {
        _json_array_push(ctx, item);
        return item;
    } else if (ctx->type == _CC_JSON_OBJECT_) {
        _json_object_push(ctx, item, true);
        return item;
    }
    _json_free_node(item);
    return nullptr;
}

/**/
_CC_API_PUBLIC(_cc_json_t*) _cc_json_add_float(_cc_json_t *ctx, const tchar_t *keyword, float64_t value) {
    _cc_json_t *item = _cc_json_alloc_object(_CC_JSON_FLOAT_, keyword);
    item->element.uni_float = value;
    if (ctx->type == _CC_JSON_ARRAY_) {
        _json_array_push(ctx, item);
        return item;
    } else if (ctx->type == _CC_JSON_OBJECT_) {
        _json_object_push(ctx, item, true);
        return item;
    }
    _json_free_node(item);
    return nullptr;
}

/**/
_CC_API_PUBLIC(_cc_json_t*) _cc_json_add_string(_cc_json_t *ctx, const tchar_t *keyword, const tchar_t *value) {
    _cc_json_t *item = _cc_json_alloc_object(_CC_JSON_STRING_, keyword);
    item->element.uni_string = _cc_sds_alloc(value,_tcslen(value));
    if (ctx->type == _CC_JSON_ARRAY_) {
        _json_array_push(ctx, item);
        return item;
    } else if (ctx->type == _CC_JSON_OBJECT_) {
        _json_object_push(ctx, item, true);
        return item;
    }
    _json_free_node(item);
    return nullptr;
}

/**/
_CC_API_PUBLIC(_cc_json_t*) _cc_json_add_sds(_cc_json_t *ctx, const tchar_t *keyword, const _cc_sds_t value) {
    _cc_json_t *item = _cc_json_alloc_object(_CC_JSON_STRING_, keyword);
    item->element.uni_string = value;
    if (ctx->type == _CC_JSON_ARRAY_) {
        _json_array_push(ctx, item);
        return item;
    } else if (ctx->type == _CC_JSON_OBJECT_) {
        _json_object_push(ctx, item, true);
        return item;
    }
    _json_free_node(item);
    return nullptr;
}

/**/
_CC_API_PRIVATE(void) _cc_json_dump_array(_cc_json_t *item, _cc_buf_t *buf);
/**/
_CC_API_PRIVATE(void) _cc_json_dump_object(_cc_json_t *item, _cc_buf_t *buf);

_CC_API_PRIVATE(void) _cc_json_dump_string(const tchar_t *output, _cc_buf_t *buf) {
    const tchar_t *p = output;
    size_t len;
    /* numbers of additional characters needed for escaping */
    size_t escape_characters = 0;

    /* empty string */
    if (output == nullptr) {
        _cc_buf_puts(buf, _T("\"\""));
        return;
    }

    /* set "flag" to 1 if something needs to be escaped */
    for (len = 0; *p; p++, len++) {
        if (char2escape[*p & 0xFF] != nullptr) {
            escape_characters++;
        }
    }

    _buf_char_put(buf, _T('"'));
    /* no characters have to be escaped */
    if (escape_characters == 0) {
        _cc_buf_append(buf, (const pvoid_t)output, sizeof(tchar_t) * len);
    } else {
        /* set "flag" to 1 if something needs to be escaped */
        for (p = output; *p; p++) {
            const tchar_t *pp = char2escape[*p & 0xFF];
            if (pp && ((int32_t)*pp) < 256) {
                _cc_buf_puts(buf, pp);
            } else {
                _buf_char_put(buf, *p);
            }
        }
    }
    _buf_char_put(buf, _T('"'));
}

/**/
static void _cc_json_dump_value(_cc_json_t *item, _cc_buf_t *buf) {
    switch (item->type) {
    case _CC_JSON_NULL_: {
        _cc_buf_append(buf, (const pvoid_t) _T("null"), 4);
    } break;
    case _CC_JSON_BOOLEAN_: {
        if (item->element.uni_boolean) {
            _cc_buf_append(buf, (const pvoid_t) _T("true"), 4);
        } else {
            _cc_buf_append(buf, (const pvoid_t) _T("false"), 5);
        }
    } break;
    case _CC_JSON_FLOAT_: {
        int length = 0;
        double test = 0;
        /* temporary buffer to print the number into */
        tchar_t number_buffer[26];

        /* This checks for NaN and Infinity */
        if ((item->element.uni_float * 0) != 0) {
            length = _sntprintf(number_buffer, _cc_countof(number_buffer), _T("null"));
        } else {
            /* Try 15 decimal places of precision to avoid nonsignificant
             * nonzero digits */
            length = _sntprintf(number_buffer, _cc_countof(number_buffer), _T("%1.15g"), item->element.uni_float);

            /* Check whether the original double can be recovered */
            if ((_stscanf(number_buffer, _T("%lg"), &test) != 1) || (test != item->element.uni_float)) {
                /* If not, print with 17 decimal places of precision */
                length = _sntprintf(number_buffer, _cc_countof(number_buffer), _T("%1.17g"), item->element.uni_float);
            }
        }

        /* sprintf failed or buffer overrun occurred */
        if ((length < 0) || (length > (int)(sizeof(number_buffer) - 1))) {
            _sntprintf(number_buffer, _cc_countof(number_buffer), _T("null"));
        }

        number_buffer[_cc_countof(number_buffer) - 1] = 0;
        _cc_buf_append(buf, number_buffer, sizeof(tchar_t) * (length - 1));
    } break;
    case _CC_JSON_INT_: {
        _cc_buf_appendf(buf, _T("%lld"), (long long)item->element.uni_int);
    } break;
    case _CC_JSON_OBJECT_: {
        _cc_json_dump_object(item, buf);
    } break;
    case _CC_JSON_ARRAY_: {
        _cc_json_dump_array(item, buf);
    } break;
    case _CC_JSON_STRING_: {
        _cc_json_dump_string(item->element.uni_string, buf);
    } break;
    default:
        break;
    }
}

/**/
_CC_API_PRIVATE(void) _cc_json_dump_array(_cc_json_t *root, _cc_buf_t *buf) {
    size_t i, length;

    _buf_char_put(buf, _JSON_ARRAY_START_);
    length = _cc_array_length(root->element.uni_array);

    for (i = 0; i < length; i++) {
        _cc_json_t *v = (_cc_json_t*)_cc_array_value(root->element.uni_array, i);
        _cc_json_dump_value(v, buf);
        _buf_char_put(buf, _JSON_NEXT_TOKEN_);
    }

    if (length > 0) {
        buf->length -= 1;
    }

    _buf_char_put(buf, _JSON_ARRAY_END_);
}
    /**/
_CC_API_PRIVATE(void) _cc_json_dump_object(_cc_json_t *root, _cc_buf_t *buf) {
    _cc_json_t *item = nullptr;
    _cc_rbtree_iterator_t *next, *head;

    _buf_char_put(buf, _JSON_OBJECT_START_);
    head = next = _cc_rbtree_first(&root->element.uni_object);
    while (next) {
        item = _cc_upcast(next, _cc_json_t, lnk);

        _cc_json_dump_string(item->name, buf);
        _cc_buf_append(buf, _T(": "), 2 * sizeof(tchar_t));

        _cc_json_dump_value(item, buf);

        _buf_char_put(buf, _JSON_NEXT_TOKEN_);

        next = _cc_rbtree_next(next);
    }

    if (head) {
        buf->bytes[buf->length - 1] = _JSON_OBJECT_END_;
    } else {
        _buf_char_put(buf, _JSON_OBJECT_START_);
    }
}

_CC_API_PUBLIC(void) _cc_json_dump(_cc_json_t *item, _cc_buf_t *buf) {
    _cc_alloc_buf(buf, _CC_16K_BUFFER_SIZE_);
    _cc_json_dump_value(item, buf);
}
