
#ifndef _C_CC_LIBSOCKS5_H_INCLUDED_
#define _C_CC_LIBSOCKS5_H_INCLUDED_

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

#define _CC_SOCKS5_AUTH_NONE_           0x00
#define _CC_SOCKS5_AUTH_GSSAPI_         0x01
#define _CC_SOCKS5_AUTH_ID_KEY_         0x02
#define _CC_SOCKS5_AUTH_IANA_           0x03
#define _CC_SOCKS5_AUTH_RESERVE_        0x04
#define _CC_SOCKS5_AUTH_NOT_SUPPORT_    0xFF

#define _CC_SOCKS5_AUTH_RESULT_OK_      0x00
#define _CC_SOCKS5_AUTH_RESULT_ERR_     0x0F

#define _CC_SOCKS5_SOCKS_CMD_CONNECT_   0x01
#define _CC_SOCKS5_SOCKS_CMD_BIND_      0x02
#define _CC_SOCKS5_SOCKS_CMD_UDPASS_    0x03

#define _CC_SOCKS5_ADDRESS_TYPE_IPV4_   0x01
#define _CC_SOCKS5_ADDRESS_TYPE_DOMAIN_ 0x03
#define _CC_SOCKS5_ADDRESS_TYPE_IPV6_   0x04

#define _CC_SOCKS5_CMD_RESPONSE_OK_             0x00
#define _CC_SOCKS5_CMD_RESPONSE_AGENT_ERR_      0x01
#define _CC_SOCKS5_CMD_RESPONSE_NOT_ALLOWED_    0x02
#define _CC_SOCKS5_CMD_RESPONSE_NETWORK_ERR_    0x03
#define _CC_SOCKS5_CMD_RESPONSE_TARGET_INVALID_ 0x04
#define _CC_SOCKS5_CMD_RESPONSE_TARGET_REFUSED_ 0x05
#define _CC_SOCKS5_CMD_RESPONSE_TTL_TIMEOUT_    0x06
#define _CC_SOCKS5_CMD_RESPONSE_NOT_SUPPORTED_  0x07
#define _CC_SOCKS5_CMD_RESPONSE_TGT_NOTSPT_     0x08

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /* _C_CC_LIBSOCKS5_H_INCLUDED_ */


