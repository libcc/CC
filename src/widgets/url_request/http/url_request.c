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
#include <libcc/widgets/url_request.h>
#include <libcc/widgets/gzip.h>

/**/
_CC_API_PUBLIC(bool_t) _cc_url_response_body(_cc_url_request_t *request, byte_t *source, size_t length);
_CC_API_PUBLIC(bool_t) _cc_url_response_chunked(_cc_url_request_t *, _cc_io_buffer_t *io);

/**/
_CC_API_WIDGETS(void) _cc_reset_url_request(_cc_url_request_t *request) {
    _cc_assert(request != nullptr);
    request->status = _CC_HTTP_STATUS_HEADER_;
    if (request->response) {
        _cc_http_free_response_header(&request->response);
    }
#ifdef _CC_USE_OPENSSL_
    if (request->io && request->io->ssl) {
        _SSL_free(request->io->ssl);
        request->io->ssl = nullptr;
    }
#endif
}

/**/
_CC_API_PUBLIC(void) _cc_free_url_request(_cc_url_request_t *request) {
    _cc_assert(request != nullptr);

    if (request->gzip) {
        _gzip_free(request->gzip);
        request->gzip = nullptr;
    }

    if (request->response) {
        _cc_http_free_response_header(&request->response);
    }

    _cc_free_buf(&request->buffer);
    _cc_free_url(&request->url);
    _cc_free(request);
}

/**/
_CC_API_PUBLIC(bool_t) _cc_url_request_header(_cc_url_request_t *request, _cc_event_t *e) {
    _cc_assert(request != nullptr);
    _cc_assert(request->io != nullptr);
    _cc_assert(request->buffer.length > 0);
    request->status = _CC_HTTP_STATUS_HEADER_;
    if (request->response) {
        _cc_http_free_response_header(&request->response);
    }
    if (request->buffer.length <= 0) {
        return false;
    }
#ifdef _CC_UNICODE_
    _cc_buf_utf16_to_utf8(&request->buffer, 0);
#endif
    return _cc_io_buffer_send(e, request->io, request->buffer.bytes, (int32_t)request->buffer.length);
}

_CC_API_PRIVATE(int64_t) get_content_length(_cc_rbtree_t *headers) {
    const _cc_http_header_t *data = _cc_http_header_find(headers, _T("Content-Length"));
    return data ? _ttoi(data->value) : 0;
}

_CC_API_PRIVATE(int) is_chunked_transfer(_cc_rbtree_t *headers) {
    const _cc_http_header_t *data = _cc_http_header_find(headers, _T("Transfer-Encoding"));
    return (data && !_tcsicmp(data->value, _T("chunked"))) ? _CC_URL_TRANSFER_ENCODING_CHUNKED_ : _CC_URL_TRANSFER_ENCODING_UNKNOWN_;
}

_CC_API_PRIVATE(int) get_content_encoding(_cc_rbtree_t *headers) {
    const _cc_http_header_t *data = _cc_http_header_find(headers, _T("Content-Encoding"));
    return (data && !_tcsicmp(data->value, _T("gzip"))) ? _CC_URL_CONTENT_ENCODING_GZIP_ : _CC_URL_CONTENT_ENCODING_PLAINTEXT_;
}

_CC_API_PRIVATE(bool_t) is_keep_alive(_cc_rbtree_t *headers) {
    const _cc_http_header_t *data = _cc_http_header_find(headers, _T("Connection"));
    return data ? !_tcsicmp(data->value, _T("keep-alive")) : false;
}

/**/
_CC_API_PUBLIC(bool_t) _cc_url_request_response_header(_cc_url_request_t *request) {
    _cc_http_response_header_t *response;
    _cc_io_buffer_t *io;
    _cc_assert(request != nullptr);
    _cc_assert(request->io != nullptr);
    _cc_assert(request->status == _CC_HTTP_STATUS_HEADER_);

    io = request->io;
    request->status = _cc_http_header_parser((_cc_http_header_fn_t)_cc_http_alloc_response_header, (pvoid_t *)&request->response, io->r.bytes, (int32_t *)&io->r.off);
    /**/
    if (request->status != _CC_HTTP_STATUS_PAYLOAD_) {
        return request->status == _CC_HTTP_STATUS_HEADER_;
    }
    _cc_buf_cleanup(&request->buffer);
    response = request->response;
    response->length = get_content_length(&response->headers);
    response->content_encoding = get_content_encoding(&response->headers);
    response->transfer_encoding = is_chunked_transfer(&response->headers);
    response->keep_alive = is_keep_alive(&response->headers);
    if (response->content_encoding == _CC_URL_CONTENT_ENCODING_GZIP_) {
        if (request->gzip) {
            _gzip_reset(request->gzip);
        } else {
            request->gzip = _gzip_alloc(_GZIP_INF_);
            return request->gzip != nullptr;
        }
    }
    return true;
}

/**/
_CC_API_PUBLIC(bool_t) _cc_url_request_response_body(_cc_url_request_t *request) {
    _cc_http_response_header_t *response;
    _cc_io_buffer_t *io;
    _cc_assert(request != nullptr);
    _cc_assert(request->io != nullptr);
    _cc_assert(request->status == _CC_HTTP_STATUS_PAYLOAD_);

    /**/
    response = request->response;
    io = request->io;

    if (io->r.off > 0) {
        if (response->transfer_encoding == _CC_URL_TRANSFER_ENCODING_CHUNKED_) {
            if (!_cc_url_response_chunked(request, io)) {
                return false;
            }
        } else {
            if (!_cc_url_response_body(request, io->r.bytes, io->r.off)) {
                return false;
            }
            /**/
            response->download_length += io->r.off;
            io->r.off = 0;
            if (response->length > 0 && response->download_length >= response->length) {
                request->status = _CC_HTTP_STATUS_ESTABLISHED_;
            }
        }
    } else if (response->length == 0 && response->transfer_encoding != _CC_URL_TRANSFER_ENCODING_CHUNKED_) {
        request->status = _CC_HTTP_STATUS_ESTABLISHED_;
    }

    return true;
}

/**/
_CC_API_PUBLIC(_cc_url_request_t*) _cc_url_request(const tchar_t *url, pvoid_t args) {
    _cc_url_request_t *request;
    _cc_assert(url != nullptr);
    if (url == nullptr) {
        return nullptr;
    }

    request = (_cc_url_request_t *)_cc_malloc(sizeof(_cc_url_request_t));
    bzero(request, sizeof(_cc_url_request_t));

    if (!_cc_parse_url(&request->url, url)) {
        _cc_free(request);
        return nullptr;
    }

    request->io = _cc_alloc_io_buffer(_CC_8K_BUFFER_SIZE_);

    request->status = _CC_HTTP_STATUS_HEADER_;
	request->handshake = _CC_SSL_HS_ESTABLISHED_;
    request->response = nullptr;
    request->args = args;
    request->gzip = nullptr;

    _cc_alloc_buf(&request->buffer, _CC_IO_BUFFER_SIZE_);

    return request;
}

_CC_API_PUBLIC(bool_t) _cc_url_request_ssl(_cc_OpenSSL_t *openSSL, _cc_url_request_t *request, _cc_event_t *e) {
#ifdef _CC_USE_OPENSSL_
    request->io->ssl = _SSL_connect(openSSL, e->fd);
    if (request->io->ssl == nullptr) {
        return false;
    }
    _SSL_set_host_name(request->io->ssl, request->url.host, _cc_sds_length(request->url.host));
#else
    _CC_UNUSED(openSSL);
    _CC_UNUSED(request);
    _CC_UNUSED(e);
#endif
    return true;
}