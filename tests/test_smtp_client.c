#include <libcc/smtp.h>
#include <libcc.h>
#include <locale.h>
#include <stdio.h>

_CC_API_PRIVATE(bool_t) _net_cb(_cc_async_event_t* async, _cc_event_t* e, const uint32_t which) {
    _cc_smtp_t* smtp = (_cc_smtp_t*)e->data;
    if (which & _CC_EVENT_CONNECT_) {
        if (!_cc_smtp_connected(smtp)) {
            return false;
        }
    } else if (which & _CC_EVENT_CLOSED_) {
        if (smtp) {
            _cc_free_smtp(smtp);
            e->data = 0;
        }
        return false;
    }

    if (which & _CC_EVENT_READABLE_) {
        int32_t off = _cc_io_buffer_read(e, smtp->io);
        if (off < 0) {
            _tprintf(_T("TCP close %d\n"), e->fd);
            return false;
        } else if (off > 0) {
            _cc_io_buffer_t *io = (_cc_io_buffer_t *)smtp->io;
            char_t *start = (char_t*)io->r.bytes;
            char_t *endpos = (char_t*)io->r.bytes + io->r.off;
            size_t offset = 0;

            *endpos = 0;
            do {
                char_t *p = memchr(start, '\n', off);
                if (p == nullptr) {
                    break;
                }
                _cc_logger_warin("%.d",(int)smtp->flag);
                if ( !smtp->response_cb(smtp, (byte_t*)start, p - start) ) {
                    _cc_logger_error("%.*s",(int)(p - start), start);
                    _cc_smtp_logout(smtp);
                    break;
                } else {
                    _cc_logger_info("%.*s",(int)(p - start), start);
                }
                offset += (p - start) + 1;
                start = p + 1;
            } while (start < endpos);

            io->r.off -= offset;
            if (io->r.off > 0) {
                memmove(io->r.bytes, &io->r.bytes + offset, io->r.off);
            }

            if (io->w.off > 0) {
                if (_cc_io_buffer_flush(e, smtp->io) < 0) {
                    _tprintf(_T("send fail %d\n"), e->fd);
                    return false;
                }
            }
        }
    }

    if (which & _CC_EVENT_WRITABLE_) {
        return _cc_io_buffer_flush(e, smtp->io) >= 0;
    }

    if (which & _CC_EVENT_TIMEOUT_) {
        _tprintf(_T("TCP timeout %d\n"), e->fd);
        return false;
    }

    return true;
}
int times = 0;
_CC_API_PRIVATE(bool_t) smtp_callback(_cc_smtp_t* smtp, uint32_t which) {
    switch (which) {
        case _CC_LIBSMTP_CONNECTED_:
            printf("CC_LIBSMTP_CONNECTED\n");
            _cc_smtp_login(smtp, "yourmail@163.com", "yourpassword");
            break;
        case _CC_LIBSMTP_LOGINED_:
            printf("CC_LIBSMTP_LOGIN\n");
            _cc_smtp_from_to(smtp, "yourmail@163.com", "libcc.cn@gmail.com");
            break;
        case _CC_LIBSMTP_LOGOUT_:
            printf("CC_LIBSMTP_LOGOUT\n");
            break;
        case _CC_LIBSMTP_SEND_EMAIL_:
            printf("CC_LIBSMTP_SEND_EMAIL\n");
            _cc_send_email(smtp, "libcc", "test libcc-smtp.", "Hey libcc!<br/><br/><p>test test libcc-smtp!</p><br/><br/>Thanks, <br/>The libcc Team");
            break;
        case _CC_LIBSMTP_SEND_EMAIL_SUCCESS_:
            printf("_CC_LIBSMTP_SEND_EMAIL_SUCCESS_\n");
            if (times == 3) {
                _cc_smtp_logout(smtp);
                break;
            }
            times++;
            _cc_smtp_from_to(smtp, "yourmail@163.com", "libcc.cn@gmail.com");
            break;
    }
    return true;
}

int main(int argc, char *const arvg[]) {
    int c;
    _cc_smtp_t *smtp;
    _cc_event_t *event;
    _cc_async_event_t *async;
    struct sockaddr_in sa;

    _cc_alloc_async_event(0, nullptr);

    async = _cc_get_async_event();
    _cc_assert(async != NULL);

    smtp = _cc_alloc_smtp(_CC_SMTP_LOGIN_MODE_PLAIN_, _CC_SMTP_HTML_, smtp_callback);
    event = _cc_event_alloc(async, _CC_EVENT_CONNECT_|_CC_EVENT_TIMEOUT_|_CC_EVENT_READABLE_);
    _cc_assert(event != NULL);
    if (event == nullptr) {
        return 0;
    }

    event->timeout = 10000;
    event->callback = _net_cb;
    event->data = (uintptr_t)smtp;

    _cc_inet_ipv4_addr(&sa, _T("smtp.163.com"), 25);

    if (!_cc_tcp_connect(async, event, (_cc_sockaddr_t *)&sa, sizeof(struct sockaddr_in))) {
        _cc_free_event(async, event);
        _cc_free_smtp(smtp);
    }

    while((c = getchar()) != 'q') {
        _cc_sleep(100);
    }
    _cc_free_async_event();
    return 0;
}
