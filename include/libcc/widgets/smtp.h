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

#ifndef _C_CC_WIDGETS_LIBSMTP_H_INCLUDED_
#define _C_CC_WIDGETS_LIBSMTP_H_INCLUDED_

#include "event.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#define _CC_LIBSMTP_VERSION_INFO  "libSTMP Release 1.0,* Copyright 2018-2019 libcc.cn@gmail.com"

enum {
    _CC_SMTP_TEXT_ = 0,
    _CC_SMTP_HTML_,
    _CC_SMTP_LOGIN_MODE_PLAIN_,
    _CC_SMTP_LOGIN_MODE_XOAUTH2
};

enum {
    _CC_LIBSMTP_RESP_PENDING_ = 0,
    _CC_LIBSMTP_RESP_CONNECTED_,
    _CC_LIBSMTP_RESP_EHLO_,
    _CC_LIBSMTP_RESP_AUTH_LOGIN_,
    _CC_LIBSMTP_RESP_LOGIN_USER_,
    _CC_LIBSMTP_RESP_LOGIN_PASSWORD_,
    _CC_LIBSMTP_RESP_FROM_,
    _CC_LIBSMTP_RESP_RCPT_TO_,
    _CC_LIBSMTP_RESP_DATA_,
    _CC_LIBSMTP_RESP_SEND_EMAIL_,
    _CC_LIBSMTP_RESP_LOGOUT_
};

enum {
    _CC_LIBSMTP_CONNECTED_ = 0,
    _CC_LIBSMTP_EHLO_,
    _CC_LIBSMTP_LOGINED_,
    _CC_LIBSMTP_LOGOUT_,
    _CC_LIBSMTP_SEND_EMAIL_,
    _CC_LIBSMTP_SEND_EMAIL_SUCCESS_,
};

typedef struct _cc_smtp _cc_smtp_t;
/**/
typedef bool_t (*_cc_smtp_response_callback_t)(_cc_smtp_t*, const byte_t *buf, uint32_t length);
typedef bool_t (*_cc_smtp_callback_t)(_cc_smtp_t*, uint32_t which);

/**/
struct _cc_smtp {
    byte_t login_mode;
        
    byte_t cmode;
    byte_t smode;
    byte_t mail_type;

    uint16_t flag;
        
    _cc_smtp_response_callback_t response_cb;
    _cc_smtp_callback_t callback;

    _cc_io_buffer_t *io;

    _cc_sds_t user;
    _cc_sds_t password;
    _cc_sds_t from;
    _cc_sds_t to;
};

/**/
void libsmtp_setup(_cc_smtp_t *smtp, uint16_t flag, _cc_smtp_response_callback_t cb);
bool_t libsmtp_command(_cc_smtp_t* smtp, const tchar_t *fmt, ...);
/**/
_CC_API_WIDGETS(_cc_smtp_t*) _cc_alloc_smtp(byte_t login_mode, byte_t mail_type, _cc_smtp_callback_t callback);
/**/
_CC_API_WIDGETS(void) _cc_free_smtp(_cc_smtp_t *smtp) ;
/**/
_CC_API_WIDGETS(bool_t) _cc_smtp_connected(_cc_smtp_t *smtp);
/**/
_CC_API_WIDGETS(bool_t) _cc_smtp_disconnected(_cc_smtp_t *smtp);
/**/
_CC_API_WIDGETS(bool_t) _cc_smtp_login(_cc_smtp_t *smtp, const char_t *user, const char_t *password);
/**/
_CC_API_WIDGETS(bool_t) _cc_smtp_logout(_cc_smtp_t *smtp);
/**/
_CC_API_WIDGETS(bool_t) _cc_smtp_from_to(_cc_smtp_t *smtp, const char_t *from, const char_t *to);
/**/
_CC_API_WIDGETS(bool_t) _cc_send_email(_cc_smtp_t *smtp, const char_t *from_name, const char_t *subject, const char_t *content);
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /* _C_CC_LIBSMTP_H_INCLUDED_ */


