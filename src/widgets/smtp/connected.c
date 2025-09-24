/**/
#include <libcc/widgets/smtp.h>

_CC_API_PRIVATE(bool_t) libsmtp_EHLO(_cc_smtp_t* smtp, const byte_t* buf, uint32_t length) {
    if (smtp->flag != _CC_LIBSMTP_RESP_EHLO_) {
        return false;
    }
    
    if (buf[0] == '2' && buf[1] == '5' && buf[2] == '0') {
        if (buf[3] == 0x20) {
            smtp->flag = _CC_LIBSMTP_RESP_PENDING_;
            smtp->callback(smtp, _CC_LIBSMTP_CONNECTED_);
        }
        return true;
    }
    return false;
}

_CC_API_PRIVATE(bool_t) sendEHLO(_cc_smtp_t* smtp) {
    char_t pcname[256] = {0};
    libsmtp_setup(smtp, _CC_LIBSMTP_RESP_EHLO_, libsmtp_EHLO);

    _cc_get_device_name(pcname, _cc_countof(pcname));

    return libsmtp_command(smtp, "EHLO %s\r\n", pcname);
}

/**/
_CC_API_PRIVATE(bool_t) libsmtp_connected(_cc_smtp_t* smtp, const byte_t* buf, uint32_t length) {
    if (smtp->flag != _CC_LIBSMTP_RESP_CONNECTED_) {
        return false;
    }
    // 220
    smtp->flag = _CC_LIBSMTP_RESP_PENDING_;
    if (buf[0] == '2' && buf[1] == '2' && buf[2] == '0') {
        return sendEHLO(smtp);
    }
    return false;
}

/**/
_CC_API_PUBLIC(bool_t) _cc_smtp_connected(_cc_smtp_t* smtp) {
    if (smtp == nullptr) {
        return false;
    }

    libsmtp_setup(smtp, _CC_LIBSMTP_RESP_CONNECTED_, libsmtp_connected);
    return true;
}

/**/
_CC_API_PUBLIC(bool_t) _cc_smtp_disconnected(_cc_smtp_t* smtp) {
    if (smtp == nullptr) {
        return false;
    }

    if (smtp->user) {
        _cc_sds_free(smtp->user);
        smtp->user = nullptr;
    }

    if (smtp->password) {
        _cc_sds_free(smtp->password);
        smtp->password = nullptr;
    }

    if (smtp->from) {
        _cc_sds_free(smtp->from);
        smtp->from = nullptr;
    }

    if (smtp->to) {
        _cc_sds_free(smtp->to);
        smtp->to = nullptr;
    }

    return true;
}
