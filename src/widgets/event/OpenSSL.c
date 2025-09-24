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
#include <libcc/rbtree.h>
#include <libcc/string.h>
#include <libcc/atomic.h>
#include <libcc/widgets/OpenSSL.h>

#ifdef _CC_USE_OPENSSL_

#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>


static _cc_atomic32_t _SSL_lock = 0;
static _cc_atomic32_t _SSL_init_refcount = 0;
static BIO_METHOD* _SSL_bio_method = nullptr;

struct _cc_SSL {
    SSL *handle;
    _cc_OpenSSL_t *ctx;
};

struct _cc_OpenSSL {
    SSL_CTX *handle;
    _cc_atomic32_t refcount;
};

#define _SSL_error(prefix) do {\
    _cc_loggerA_error("%s failed: %s", prefix, ERR_reason_error_string(ERR_get_error()));\
} while(0)

/*
_CC_API_PRIVATE(void) _SSL_clear_error() {
    while (ERR_peek_error()) {
        _cc_logger_error(_T("ignoring stale global SSL error"));
    }
    ERR_clear_error();
}*/

_CC_API_PRIVATE(bool_t) _SSL_only_init() {
    srand((unsigned)time(nullptr));
#if OPENSSL_VERSION_NUMBER >= 0x10100003L
    OPENSSL_init_ssl(OPENSSL_INIT_SSL_DEFAULT, nullptr);

    if (OPENSSL_init_ssl(OPENSSL_INIT_LOAD_CONFIG, nullptr) == 0) {
        _SSL_error("OPENSSL_init_ssl");
        return false;
    }
    /*
     * OPENSSL_init_ssl() may leave errors in the error queue
     * while returning success
     */
    ERR_clear_error();
#else
/* Enable configuring OpenSSL using the standard openssl.cnf
 * OPENSSL_config()/OPENSSL_init_crypto() should be the first
 * call to the OpenSSL* library.
 *  - OPENSSL_config() should be used for OpenSSL versions < 1.1.0
 *  - OPENSSL_init_crypto() should be used for OpenSSL versions >= 1.1.0
 */
#if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0
    OPENSSL_config(nullptr);
#else
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CONFIG, nullptr);
#endif
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();

    if (SSL_library_init() < 0) {
        _SSL_error("SSL_library_init");
        return false;
    }
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
#endif

    _SSL_bio_method = BIO_meth_new(BIO_TYPE_SOURCE_SINK | 100, "ssl_bio");

#if OPENSSL_VERSION_NUMBER >= 0x0090800fL
#ifndef SSL_OP_NO_COMPRESSION
    {
        /*
         * Disable gzip compression in OpenSSL prior to 1.0.0 version,
         * this saves about 522K per connection.
         */
        int n;
        STACK_OF(SSL_COMP) * ssl_comp_methods;

        ssl_comp_methods = SSL_COMP_get_compression_methods();
        n = sk_SSL_COMP_num(ssl_comp_methods);

        while (n--) {
            sk_SSL_COMP_pop(ssl_comp_methods);
        }
    }
#endif
#endif
    if (!RAND_poll()) {
        _cc_logger_warin(_T("OpenSSL: Failed to seed random number generator."));
    }

    while (RAND_status() == 0) {
        unsigned short rand_ret = rand() % 65536;
        RAND_seed(&rand_ret, sizeof(rand_ret));
    }
    return true;
}

/**/
_CC_API_PUBLIC(_cc_OpenSSL_t*) _SSL_init(bool_t is_client) {
    SSL_CTX *ssl_ctx;
    _cc_OpenSSL_t *ctx;
    long mode;
    /*SSL init*/
    if (_cc_atomic32_inc_ref(&_SSL_init_refcount)) {
        _cc_lock(&_SSL_lock, 1, _CC_LOCK_SPIN_);
        if (!_SSL_only_init()) {
            _cc_unlock(&_SSL_lock);
            return nullptr;
        }
        _cc_unlock(&_SSL_lock);
    }

    ctx = _cc_malloc(sizeof(_cc_OpenSSL_t));
    _cc_lock(&_SSL_lock, 1, _CC_LOCK_SPIN_);
    //ssl_ctx = SSL_CTX_new(is_client ? TLS_client_method() : TLS_server_method());
    ssl_ctx = SSL_CTX_new(is_client ? SSLv23_client_method() : SSLv23_server_method());
    _cc_unlock(&_SSL_lock);
    if (ssl_ctx == nullptr) {
        _SSL_error("SSL_CTX_new");
        _cc_free(ctx);
        return nullptr;
    }

    mode = SSL_CTX_get_mode(ssl_ctx);

#if defined(SSL_MODE_RELEASE_BUFFERS)
    mode |= SSL_MODE_RELEASE_BUFFERS;
#endif
    SSL_CTX_set_mode(ssl_ctx, SSL_MODE_ENABLE_PARTIAL_WRITE | mode);
    if (is_client) {
        SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_NONE, nullptr);
        SSL_CTX_set_quiet_shutdown(ssl_ctx, 1);
    }

    ctx->handle = ssl_ctx;
    ctx->refcount = 0;
    return ctx;
}

_CC_API_PUBLIC(void) _SSL_quit(_cc_OpenSSL_t *ctx) {
    if (_cc_atomic32_dec_ref(&_SSL_init_refcount)) {
        if (_SSL_bio_method) {
            BIO_meth_free(_SSL_bio_method);
            _SSL_bio_method = nullptr;
        }
    #if OPENSSL_VERSION_NUMBER < OPENSSL_VERSION_1_1_0
        CONF_modules_free();
        ENGINE_cleanup();
        EVP_cleanup();
        CRYPTO_cleanup_all_ex_data();
        ERR_free_strings();
    #if OPENSSL_VERSION_NUMBER >= OPENSSL_VERSION_1_0_2
        SSL_COMP_free_compression_methods();
    #endif
    #endif
    }

    SSL_CTX_free(ctx->handle);
    ctx->handle = nullptr;
    _cc_free(ctx);
}

/**/
_CC_API_PUBLIC(bool_t) _SSL_free(_cc_SSL_t *ssl) {
    if (ssl->handle == nullptr || ssl->ctx == nullptr) {
        return true;
    }

    if (!SSL_in_init(ssl->handle) && ssl->ctx->handle) {
        _cc_lock(&_SSL_lock, 1, _CC_LOCK_SPIN_);
        SSL_CTX_remove_session(ssl->ctx->handle, SSL_get0_session(ssl->handle));
        _SSL_lock = 0;
    }

    _cc_atomic32_dec(&ssl->ctx->refcount);

    SSL_shutdown(ssl->handle);
    SSL_free(ssl->handle);

    _cc_free(ssl);

    ERR_clear_error();
    return true;
}

/**/
_CC_API_PUBLIC(_cc_SSL_t*) _SSL_alloc(_cc_OpenSSL_t* ctx) {
    _cc_SSL_t *ssl = _cc_malloc(sizeof(_cc_SSL_t));

    _cc_lock(&_SSL_lock, 1, _CC_LOCK_SPIN_);
    ssl->handle = SSL_new(ctx->handle);
    _SSL_lock = 0;

    if (ssl->handle == nullptr) {
        _cc_free(ssl);
        return nullptr;
    }

    ssl->ctx = ctx;

    return ssl;
}

_CC_API_PUBLIC(bool_t) _SSL_setup(_cc_OpenSSL_t *ssl,
                                const tchar_t *cert_file,
                                const tchar_t *key_file,
                                const tchar_t *key_password) {

    //SSL_CTX_set_verify(ssl->handle, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, nullptr);
    SSL_CTX_set_verify(ssl->handle, SSL_VERIFY_NONE, nullptr);

    if (ssl) {
        static _cc_atomic64_t session_id = 0;
        uint64_t session_id_context = _cc_atomic64_inc(&session_id);
        SSL_CTX_set_session_id_context(ssl->handle, (byte_t*)&session_id_context, sizeof(session_id_context));
    }

    if (key_password) {
        SSL_CTX_set_default_passwd_cb_userdata(ssl->handle, (void*)key_password);
    }

    if (SSL_CTX_use_PrivateKey_file(ssl->handle, key_file, SSL_FILETYPE_PEM) <= 0) {
        _cc_logger_error(_T("SSL_CTX_use_PrivateKey_file failed: %s\n"), ERR_reason_error_string(ERR_get_error()));
        return false;
    }

    if (SSL_CTX_use_certificate_file(ssl->handle, cert_file, SSL_FILETYPE_PEM) <= 0) {
        _cc_logger_error(_T("SSL_CTX_use_certificate_file failed: %s\n"), ERR_reason_error_string(ERR_get_error()));
        return false;
    }

    if (!SSL_CTX_check_private_key(ssl->handle)) {
        _cc_logger_error(_T("SSL_CTX_check_private_key failed: %s\n"), ERR_reason_error_string(ERR_get_error()));
        return false;
    }
    return true;
}

_CC_API_PUBLIC(void) _SSL_set_host_name(_cc_SSL_t *ssl, tchar_t *host, size_t length) {
#ifdef _CC_UNICODE_
    char host_utf8[256];
#endif
#ifdef _CC_UNICODE_
    _cc_utf16_to_utf8((uint16_t *)host, (uint16_t *)(host + length), (uint8_t *)host_utf8,
                      (uint8_t *)host_utf8 + _cc_countof(host_utf8));
    SSL_set_tlsext_host_name(ssl->handle, host_utf8);
#else
    SSL_set_tlsext_host_name(ssl->handle, host);
#endif
}

_CC_API_PRIVATE(uint8_t) _SSL_Error(SSL *handle,const char *fn) {
	switch (SSL_get_error(handle, 0)) {
        case SSL_ERROR_WANT_READ:
            return _CC_SSL_HS_WANT_READ_;
        case SSL_ERROR_WANT_WRITE:
            return _CC_SSL_HS_WANT_WRITE_;
		case SSL_ERROR_SYSCALL:{
            int err = _cc_last_errno();
            if ((err == _CC_EINTR_ || err == _CC_EAGAIN_)) {
                return _CC_SSL_HS_SYSCALL_WOULDBLOCK_;
            }
			_cc_logger_error(_T("SSL_get_error failed:%s."), _cc_last_error(_cc_last_errno()));
			return _CC_SSL_HS_ERROR_;
		}
	}
	_SSL_error(fn);
    return _CC_SSL_HS_ERROR_;
}

_CC_API_PUBLIC(_cc_SSL_t*) _SSL_accept(_cc_OpenSSL_t *ctx, _cc_socket_t fd) {
    _cc_SSL_t *ssl = _SSL_alloc(ctx);

    if (ssl) {
		SSL_set_fd(ssl->handle, (int)fd);
		SSL_set_accept_state(ssl->handle);
	}
	return ssl;
}

/**/
_CC_API_PUBLIC(_cc_SSL_t*) _SSL_connect(_cc_OpenSSL_t *ctx, _cc_socket_t fd) {
    _cc_SSL_t *ssl = _SSL_alloc(ctx);
    if (ssl) {
		SSL_set_fd(ssl->handle, (int)fd);
		SSL_set_connect_state(ssl->handle);
	}
	return ssl;
}

/**/
_CC_API_PUBLIC(uint8_t) _SSL_do_handshake(_cc_SSL_t* ssl) {
    ERR_clear_error();
    if (SSL_do_handshake(ssl->handle) == 1) {
		return _CC_SSL_HS_ESTABLISHED_;
    }
    return _SSL_Error(ssl->handle, _CC_FUNC_);
}

/**/
_CC_API_PUBLIC(int32_t) _SSL_send(_cc_SSL_t *ssl, const byte_t *buf, int32_t length) {
    int32_t rc = 0;
	_cc_assert(buf != nullptr);
	_cc_assert(length > 0);

    ERR_clear_error();
    rc = (int32_t)SSL_write(ssl->handle, (char *)buf, length);
    if (rc <= 0) {
		if (_SSL_Error(ssl->handle, _CC_FUNC_) != _CC_SSL_HS_ERROR_) {
			return 0;
		}
    }
    return rc;
}

/**/
_CC_API_PUBLIC(int32_t) _SSL_read(_cc_SSL_t *ssl, byte_t *buf, int32_t length) {
    int32_t rc;

    ERR_clear_error();
    rc = (int32_t)SSL_read(ssl->handle, buf, length);
    if (rc > 0) {
        return rc;
    } else if (rc == 0) {
        return -1;
    }
	if (_SSL_Error(ssl->handle, _CC_FUNC_) != _CC_SSL_HS_ERROR_) {
		return 0;
	}
    return rc;
}

#endif /*OpenSSL*/