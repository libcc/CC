#include <libcc/crypto/base64.h>
#include <libcc/smtp.h>

_CC_API_PRIVATE(bool_t) libsmtp_quit_user(_cc_smtp_t* smtp, const byte_t* buf, uint32_t length) {
    if (smtp->flag != _CC_LIBSMTP_RESP_LOGOUT_) {
        return false;
    }

    smtp->flag = _CC_LIBSMTP_RESP_PENDING_;

    if (buf[0] == '2' && buf[1] == '2' && buf[2] == '1') {
        return smtp->callback(smtp, _CC_LIBSMTP_LOGOUT_);
    }
    return false;
}

_CC_API_PRIVATE(bool_t) libsmtp_login_password(_cc_smtp_t* smtp, const byte_t* buf, uint32_t length) {
    if (smtp->flag != _CC_LIBSMTP_RESP_LOGIN_PASSWORD_) {
        return false;
    }

    smtp->flag = _CC_LIBSMTP_RESP_PENDING_;

    if (buf[0] == '2' && buf[1] == '3' && buf[2] == '5') {
        return smtp->callback(smtp, _CC_LIBSMTP_LOGINED_);
    }
    return false;
}

_CC_API_PRIVATE(bool_t) libsmtp_login_user(_cc_smtp_t* smtp, const byte_t* buf, uint32_t length) {
    _cc_io_buffer_t *io = (_cc_io_buffer_t *)smtp->io;
    if (smtp->flag != _CC_LIBSMTP_RESP_LOGIN_USER_) {
        return false;
    }
    smtp->flag = _CC_LIBSMTP_RESP_PENDING_;
    // 331 Please specify the password.
    if (buf[0] == '3' && buf[1] == '3' && buf[2] == '4') {
        libsmtp_setup(smtp, _CC_LIBSMTP_RESP_LOGIN_PASSWORD_,libsmtp_login_password);
        io->w.off += _snprintf((char*)(io->w.bytes + io->w.off), io->w.limit, "%s\r\n", smtp->password);
        return true;
    }
    return false;
}

_CC_API_PRIVATE(bool_t) libsmtp_auth_login(_cc_smtp_t* smtp, const byte_t* buf, uint32_t length) {
    _cc_io_buffer_t *io = (_cc_io_buffer_t *)smtp->io;
    if (smtp->flag != _CC_LIBSMTP_RESP_AUTH_LOGIN_) {
        return false;
    }

    smtp->flag = _CC_LIBSMTP_RESP_PENDING_;
    if (buf[0] == '3' && buf[1] == '3' && buf[2] == '4') {
        libsmtp_setup(smtp, _CC_LIBSMTP_RESP_LOGIN_USER_, libsmtp_login_user);
        io->w.off += _snprintf((char*)(io->w.bytes + io->w.off), io->w.limit, "%s\r\n", smtp->user);
        return true;
    }
    return false;
}

_CC_API_PRIVATE(bool_t) libsmtp_xauth2_login(_cc_smtp_t* smtp, const byte_t* buf, uint32_t length) {
    _cc_io_buffer_t *io = (_cc_io_buffer_t *)smtp->io;
    if (smtp->flag != _CC_LIBSMTP_RESP_AUTH_LOGIN_) {
        return false;
    }

    smtp->flag = _CC_LIBSMTP_RESP_PENDING_;
    if (buf[0] == '3' && buf[1] == '3' && buf[2] == '4') {
        libsmtp_setup(smtp, _CC_LIBSMTP_RESP_LOGIN_USER_, libsmtp_login_user);
        io->w.off += _snprintf((char*)(io->w.bytes + io->w.off), io->w.limit, "%s\r\n", smtp->user);
    }
    return false;
}

_CC_API_PUBLIC(bool_t) _cc_smtp_login(_cc_smtp_t* smtp, const char_t* user, const char_t* password) {
    _cc_io_buffer_t *io;
    _cc_assert(smtp != nullptr);
    _cc_assert(smtp->io != nullptr);
    _cc_assert(user != nullptr);
    _cc_assert(password != nullptr);

    if (smtp == nullptr) {
        return false;
    }

    if (smtp->io == nullptr) {
        _cc_logger_error(_T("Not connected to SMTP server"));
        return false;
    }

    if (smtp->flag != _CC_LIBSMTP_RESP_PENDING_) {
        return false;
    }

    io = (_cc_io_buffer_t *)smtp->io;
    if (smtp->login_mode == _CC_SMTP_LOGIN_MODE_PLAIN_) {
        if (user) {
            size_t ulen = strlen(user);
            size_t xlen = sizeof(char_t) * _CC_BASE64_EN_LEN(ulen);
            smtp->user = _cc_sds_alloc(nullptr, xlen);
            xlen = _cc_base64_encode((byte_t*)user, ulen, smtp->user, xlen);
            _cc_sds_set_length(smtp->user, xlen);
        }
        if (password) {
            size_t ulen = strlen(password);
            size_t xlen = sizeof(char_t) * _CC_BASE64_EN_LEN(ulen);
            smtp->password = _cc_sds_alloc(nullptr, xlen);
            xlen = _cc_base64_encode((byte_t*)password, ulen, smtp->password, xlen);
            _cc_sds_set_length(smtp->password, xlen);
        }
        
        libsmtp_setup(smtp, _CC_LIBSMTP_RESP_AUTH_LOGIN_, libsmtp_auth_login);
        memcpy(io->w.bytes + io->w.off, "AUTH LOGIN\r\n", 12 * sizeof(char_t));
        io->w.off += 12;
        return true;
    } else {
        char_t auth[10240];
        size_t length = snprintf(auth,_cc_countof(auth), "user=%s%cauth=Bearer %s%c%c", user, 0x01, password, 0x01, 0x01);
        size_t xlen = sizeof(char_t) * _CC_BASE64_EN_LEN(length);
        smtp->user = (char_t*)_cc_malloc(xlen);
        length = _cc_base64_encode((byte_t*)auth, length, smtp->password, xlen);

        libsmtp_setup(smtp, _CC_LIBSMTP_RESP_AUTH_LOGIN_, libsmtp_xauth2_login);
        memcpy(io->w.bytes + io->w.off, "AUTH XOAUTH2\r\n", 14 * sizeof(char_t));
        io->w.off += 14;
        return true;
    }
    return false;
}

_CC_API_PUBLIC(bool_t) _cc_smtp_logout(_cc_smtp_t* smtp) {
    _cc_io_buffer_t *io;
    _cc_assert(smtp != nullptr && smtp->io != nullptr);

    if (smtp == nullptr || smtp->io == nullptr) {
        return false;
    }

    libsmtp_setup(smtp, _CC_LIBSMTP_RESP_LOGOUT_, libsmtp_quit_user);

    io = (_cc_io_buffer_t *)smtp->io;
    memcpy(io->w.bytes + io->w.off, (byte_t*)"QUIT\r\n", 6 * sizeof(char_t));
    io->w.off += 6;
    return true;
}
