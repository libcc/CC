
#include <libcc/widgets/smtp.h>

/**/
void libsmtp_setup(_cc_smtp_t* smtp, uint16_t flag, _cc_smtp_response_callback_t cb) {
    smtp->flag = flag;
    smtp->response_cb = cb;
}

bool_t libsmtp_command(_cc_smtp_t* smtp, const tchar_t *fmt, ...) {
    int32_t off,avail;
    va_list arg;
    _cc_io_buffer_t *io = (_cc_io_buffer_t *)smtp->io;

    _cc_assert(fmt != nullptr);

    va_start(arg, fmt);
    avail = io->w.limit - io->w.off;
    off = vsnprintf((char*)(io->w.bytes + io->w.off), avail, fmt, arg);
    if (off > 0 && off < avail) {
        io->w.off += off;
    }
    va_end(arg);
    return true;
}

_CC_API_PRIVATE(bool_t) libsmtp_send_email(_cc_smtp_t* smtp, const byte_t* buf, uint32_t length) {
    if (smtp->flag != _CC_LIBSMTP_RESP_SEND_EMAIL_) {
        return false;
    }

    smtp->flag = _CC_LIBSMTP_RESP_PENDING_;

    if (buf[0] == '2' && buf[1] == '5' && buf[2] == '0') {
        smtp->callback(smtp, _CC_LIBSMTP_SEND_EMAIL_SUCCESS_);
        return true;
    }
    return false;
}

/**/
_CC_API_PUBLIC(bool_t) _cc_send_email(_cc_smtp_t* smtp,
                      const char_t* from_name,
                      const char_t* subject,
                      const char_t* content) {
    char_t date[128];
    int32_t length_of_content;
    int32_t required_length;
    char* content_type = "text/plain;charset=UTF-8";
    time_t seconds = time(nullptr);
    struct tm* m = gmtime(&seconds);
    _cc_io_buffer_t *io = (_cc_io_buffer_t *)smtp->io;

    _cc_assert(smtp != nullptr);
    _cc_assert(subject != nullptr);
    _cc_assert(content != nullptr);

    if (smtp == nullptr) {
        return false;
    }

    if (smtp->flag != _CC_LIBSMTP_RESP_PENDING_) {
        return false;
    }

    if (smtp->mail_type == _CC_SMTP_HTML_) {
        content_type = "text/html;charset=UTF-8";
    }

    libsmtp_setup(smtp, _CC_LIBSMTP_RESP_SEND_EMAIL_, libsmtp_send_email);

    strftime(date, _cc_countof(date), "%a, %d %b %Y %H:%M:%S GMT", m);
    libsmtp_command(smtp, "FROM:\"%s\"<%s>\r\n"
                  "TO:<%s>\r\n"
                  "SUBJECT:%s\r\n"
                  "Date:%s\r\n"
                  "MIME-Version:1.0\r\n"
                  "X-Mailer:SMTP\r\n"
                  "Content-type: %s\r\n\r\n",
                  from_name, smtp->from, smtp->to, subject, date, content_type);

    length_of_content = strlen(content);
    
    required_length = length_of_content + 5 + io->w.off;
    if (required_length > io->w.limit) {
        _cc_logger_error(_T("email content too long. length:%d > limit:%d"), length_of_content, io->w.limit - 5 - io->w.off);
        return false;
    }
    memcpy(&io->w.bytes[io->w.off], content, length_of_content);
    io->w.off += length_of_content;
    memcpy(&io->w.bytes[io->w.off], "\r\n.\r\n", 5);
    io->w.off += 5;

    return true;
}


_CC_API_PUBLIC(_cc_smtp_t *) _cc_alloc_smtp(byte_t login_mode, byte_t mail_type, _cc_smtp_callback_t callback) {
    _cc_smtp_t *smtp = (_cc_smtp_t *)_cc_calloc(1, sizeof(_cc_smtp_t));
    smtp->callback = callback;
    smtp->login_mode = login_mode;
    smtp->mail_type = mail_type;
    smtp->io = _cc_alloc_io_buffer(_CC_1K_BUFFER_SIZE_);
    
    smtp->user = nullptr;
    smtp->password = nullptr;
    smtp->from = nullptr;
    smtp->to = nullptr;

    return smtp;
}

_CC_API_PUBLIC(void) _cc_free_smtp(_cc_smtp_t *smtp) {
    _cc_free_io_buffer(smtp->io);
    _cc_free(smtp);
}