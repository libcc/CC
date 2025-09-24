
#include <libcc/widgets/smtp.h>

_CC_API_PRIVATE(bool_t) libsmtp_data(_cc_smtp_t* smtp, const byte_t* buf, uint32_t length) {
    if (smtp->flag != _CC_LIBSMTP_RESP_DATA_) {
        return false;
    }

    smtp->flag = _CC_LIBSMTP_RESP_PENDING_;

    if (buf[0] == '3' && buf[1] == '5' && buf[2] == '4') {
        smtp->callback(smtp, _CC_LIBSMTP_SEND_EMAIL_);
        return true;
    }
    return false;
}

_CC_API_PRIVATE(bool_t) libsmtp_rcpt_to(_cc_smtp_t* smtp, const byte_t* buf, uint32_t length) {
    _cc_io_buffer_t *io;
    if (smtp->flag != _CC_LIBSMTP_RESP_RCPT_TO_) {
        return false;
    }

    smtp->flag = _CC_LIBSMTP_RESP_PENDING_;

    io = (_cc_io_buffer_t *)smtp->io;
    if (buf[0] == '2' && buf[1] == '5' && buf[2] == '0') {
        libsmtp_setup(smtp, _CC_LIBSMTP_RESP_DATA_, libsmtp_data);
        memcpy(io->w.bytes + io->w.off, "DATA\r\n", 6);
        io->w.off += 6;
        return true;
    }
    return false;
}

_CC_API_PRIVATE(bool_t) libsmtp_mail_from(_cc_smtp_t* smtp, const byte_t* buf, uint32_t length) {
    if (smtp->flag != _CC_LIBSMTP_RESP_FROM_) {
        return false;
    }

    smtp->flag = _CC_LIBSMTP_RESP_PENDING_;

    if (buf[0] == '2' && buf[1] == '5' && buf[2] == '0') {
        libsmtp_setup(smtp, _CC_LIBSMTP_RESP_RCPT_TO_, libsmtp_rcpt_to);
        return libsmtp_command(smtp, "RCPT TO:<%s>\r\n", smtp->to);
    }
    return false;
}

_CC_API_PUBLIC(bool_t) _cc_smtp_from_to(_cc_smtp_t* smtp, const char_t* from, const char_t* to) {
    _cc_assert(smtp != nullptr);
    _cc_assert(smtp->io != nullptr);
    _cc_assert(from != nullptr && to != nullptr);

    if (smtp->flag != _CC_LIBSMTP_RESP_PENDING_) {
        return false;
    }

    smtp->from = _cc_sds_alloc(from,0);
    smtp->to = _cc_sds_alloc(to,0);

    libsmtp_setup(smtp, _CC_LIBSMTP_RESP_FROM_, libsmtp_mail_from);

    return libsmtp_command(smtp, "MAIL FROM:<%s>\r\n", smtp->from);
}
