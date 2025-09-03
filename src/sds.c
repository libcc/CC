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

#include <libcc/alloc.h>
#include <libcc/sds.h>

_CC_API_PRIVATE(byte_t) sds_hdr(byte_t type) {
    switch(type & _SDS_MASK_) {
        case _SDS_MASK_5_:
            return (byte_t)sizeof(struct _sds_hdr5);
        case _SDS_MASK_8_:
            return (byte_t)sizeof(struct _sds_hdr8);
        case _SDS_MASK_16_:
            return (byte_t)sizeof(struct _sds_hdr16);
        case _SDS_MASK_32_:
            return (byte_t)sizeof(struct _sds_hdr32);
        case _SDS_MASK_64_:
            return (byte_t)sizeof(struct _sds_hdr64);
    }
    return 0;
}

_CC_API_PRIVATE(byte_t) sds_rtype(size_t length) {
    if (length < (1 << 5)) {
        return _SDS_MASK_5_;
    }
    if (length < (1 << 8)) {
        return _SDS_MASK_8_;
    }
    if (length < (1 << 16)) {
        return _SDS_MASK_16_;
    }
#if (LONG_MAX == LLONG_MAX)
    if (length < (1ll << 32)) {
        return _SDS_MASK_32_;
    }
    return _SDS_MASK_64_;
#else
    return _SDS_MASK_32_;
#endif
}

_CC_API_PUBLIC(_cc_sds_t) _cc_sds_empty_alloc(size_t size) {
    return _cc_sds_alloc(nullptr, size);
}


_CC_API_PUBLIC(_cc_sds_t) _cc_sds_alloc(const tchar_t *s, size_t length) {
    byte_t type = sds_rtype(length);
    byte_t hdr_length = sds_hdr(type);
    byte_t *hdr = (byte_t*)_cc_malloc(hdr_length + (length + 1) * sizeof(tchar_t));
    byte_t *ptr = hdr + hdr_length;

    size_t ts = 0;
    if (s == nullptr) {
        *(tchar_t*)(ptr) = '\0';
    } else {
        memcpy(ptr, s, sizeof(tchar_t) * length);
        *(tchar_t*)(ptr + length) = '\0';
        ts = length;
    }

    /* flags pointer. */
    *(byte_t*)(ptr - sizeof(byte_t)) = type;

    switch (type) {
        case _SDS_MASK_5_: {
            struct _sds_hdr5 *h = (struct _sds_hdr5 *)hdr;
            h->flags |= (ts << _SDS_BITS_);
            break;
        }
        case _SDS_MASK_8_: {
            struct _sds_hdr8 *h = (struct _sds_hdr8 *)hdr;
            h->length = (uint8_t)ts;
            h->limit = (uint8_t)length;
            break;
        }
        case _SDS_MASK_16_: {
            struct _sds_hdr16 *h = (struct _sds_hdr16 *)hdr;
            h->length = (uint16_t)ts;
            h->limit = (uint16_t)length;
            break;
        }
        case _SDS_MASK_32_: {
            struct _sds_hdr32 *h = (struct _sds_hdr32 *)hdr;
            h->length = (uint32_t)ts;
            h->limit = (uint32_t)length;
            break;
        }
        case _SDS_MASK_64_: {
            struct _sds_hdr64 *h = (struct _sds_hdr64 *)hdr;
            h->length = (uint64_t)ts;
            h->limit = (uint64_t)length;
            break;
        }
    }

    return (_cc_sds_t)ptr;
}

_CC_API_PUBLIC(void) _cc_sds_free(_cc_sds_t s) {
    byte_t *hdr = (byte_t*)s;
    if (hdr == nullptr) {
        return;
    }
    _cc_free(hdr - sds_hdr(*(byte_t*)(hdr - 1)));
}

_CC_API_PUBLIC(_cc_sds_t) _cc_sds_vformat(const tchar_t *format, va_list ap) {
    size_t length;
    size_t free_length = 10240;
    tchar_t static_buf[10240];
    tchar_t *ptr = static_buf;
    _cc_sds_t s = nullptr;

    if (format == nullptr) {
        return nullptr;
    }

    /* If the first attempt to format fails, resize the buffer appropriately
     * and try again */
    while (1) {
        length = (size_t)_vsntprintf(ptr, free_length, format, ap);
    #ifdef __CC_WINDOWS__
        if (length == -1) {
            length = _vsntprintf(nullptr, 0, format, ap);
        }
    #endif
        if (length <= 0) {
            break;
        }

        /* SUCCESS */
        if (length < free_length) {
            s = _cc_sds_alloc(ptr, length);
            break;
        }

        if (ptr != static_buf) {
            _cc_free(ptr);
        }

        free_length = _cc_aligned_alloc_opt((length + 64) + sizeof(tchar_t), 64);
        ptr = (tchar_t *)_cc_malloc(free_length);
    }

    if (ptr != static_buf) {
        _cc_free(ptr);
    }
    
    return s;
}

_CC_API_PUBLIC(_cc_sds_t) _cc_sds_format(const tchar_t *format, ...){
    va_list ap;
    _cc_sds_t s;

    va_start(ap, format);
    s = _cc_sds_vformat(format, ap);
    va_end(ap);

    return s;
}

_CC_API_PUBLIC(_cc_sds_t) _cc_sds_cat(_cc_sds_t s, const tchar_t *t, size_t length) {
    if (s == nullptr) {
        s = _cc_sds_alloc(t, length);
    } else if (length <= _cc_sds_available(s)) {
        size_t curlen = _cc_sds_length(s);
        memcpy(s + curlen, t, sizeof(tchar_t) * length);
        /* null-terminate */
        s[curlen + length] = '\0';
        _cc_sds_set_length(s, curlen + length);
    } else {
        size_t curlen = _cc_sds_length(s);
        size_t newlen = curlen + length;
        _cc_sds_t ss = _cc_sds_alloc(nullptr, curlen + length);
        memcpy(ss, s, sizeof(tchar_t) * curlen);
        memcpy(ss + curlen, t, sizeof(tchar_t) * length);
        /* null-terminate */
        ss[newlen] = '\0';
        _cc_sds_set_length(ss, newlen);
        _cc_sds_free(s);
        s = ss;
    }
    return s;
}