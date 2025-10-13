#ifndef _C_CC_XML_H_INCLUDED_
#define _C_CC_XML_H_INCLUDED_

#include "sds.h"
#include "list.h"
#include "rbtree.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif
typedef struct _cc_XML_attr {
    _cc_sds_t name;
    _cc_sds_t value;

    _cc_rbtree_iterator_t lnk;
} _cc_xml_attr_t;

/**
 * @brief XML Types:
 */
enum _CC_XML_TYPES_ {
    _CC_XML_NULL_ = 0,
    _CC_XML_COMMENT_,
    _CC_XML_CONTEXT_,
    _CC_XML_DOCTYPE_,
    _CC_XML_CHILD_
};

typedef struct _cc_xml_context {
    byte_t cdata;
    _cc_sds_t text;
} _cc_xml_context_t;

typedef struct _cc_xml {
    /* The type of the item, as above. */
    byte_t type;

    _cc_sds_t name;

    union {
        _cc_sds_t uni_comment;
        _cc_sds_t uni_doctype;
        _cc_xml_context_t uni_context;
        _cc_list_iterator_t uni_child;
    } element;

    _cc_list_iterator_t lnk;
    _cc_rbtree_t attr;
} _cc_xml_t;

/**
 * @brief Open XML file
 *
 * @param file_name XML file path
 *
 * @return _cc_xml_t structure
 */
_CC_API_PUBLIC(_cc_xml_t*) _cc_xml_from_file(const tchar_t* file_name);

/**
 * @brief Parse XML string
 *
 * @param src XML string
 *
 * @return _cc_xml_t structure
 */
_CC_API_PUBLIC(_cc_xml_t*) _cc_xml_parse(const tchar_t* src, size_t length);
/**
 * @brief Parse XML string
 *
 * @return _cc_xml_t structure
 */
_CC_API_PUBLIC(_cc_xml_t*) _cc_xml_parser(_cc_sbuf_t *const buffer);
/**
 * @brief Destroy XML object
 *
 * @param ctx _cc_xml_t structure
 *
 */
_CC_API_PUBLIC(void) _cc_free_xml(_cc_xml_t* ctx);

/**
 * @brief Print XML Object to buffer
 *
 * @param ctx _cc_xml_t structure
 *
 */
_CC_API_PUBLIC(void) _cc_dump_xml(_cc_xml_t* ctx, _cc_buf_t* buf);

/**
 * @brief new a XML object
 *
 * @param type type = _CC_XML_TYPES_
 *
 * @return _cc_xml_t structure
 */
_CC_API_PUBLIC(_cc_xml_t*) _cc_new_xml_element(byte_t type);
/**
 * @brief Append XML
 *
 * @param ctx _cc_xml_t structure
 * @param child _cc_xml_t structure
 *
 * @return true if successful or false on error.
 */
_CC_API_PUBLIC(bool_t) _cc_xml_element_append(_cc_xml_t* ctx, _cc_xml_t* child);
/**
 * @brief Get XML first Object
 *
 * @param ctx _cc_xml_t structure
 *
 * @return _cc_xml_t structure
 */
_CC_API_PUBLIC(_cc_xml_t*) _cc_xml_element_first_child(_cc_xml_t* ctx);
/**
 * @brief Get XML next Object
 *
 * @param ctx _cc_xml_t structure
 *
 * @return _cc_xml_t structure
 */
_CC_API_PUBLIC(_cc_xml_t*) _cc_xml_element_next_child(_cc_xml_t* ctx);
/**
 * @brief Find XML Object
 *
 * @param ctx _cc_xml_t structure
 * @param item XMLNode Name
 *
 * @return _cc_xml_t structure
 */
_CC_API_PUBLIC(_cc_xml_t*) _cc_xml_element_find(_cc_xml_t* ctx, tchar_t* item);
/**
 * @brief Get XML text
 *
 * @param ctx _cc_xml_t structure
 *
 * @return text
 */
_CC_API_PUBLIC(const _cc_sds_t) _cc_xml_element_text(_cc_xml_t* ctx);
/**
 * @brief Get XML attribute value
 *
 * @param ctx _cc_xml_t structure
 * @param keyword Keywords for XML attribute names
 *
 * @return attribute value
 */
_CC_API_PUBLIC(const _cc_sds_t)
_cc_xml_element_attr(_cc_xml_t* ctx, const tchar_t* keyword);

/**
 * @brief Set XML attribute value
 *
 * @param ctx _cc_xml_t structure
 * @param keyword XML attribute name
 * @param value XML attribute value
 *
 * @return true if successful or false on error.
 */
_CC_API_PUBLIC(bool_t)
_cc_xml_element_set_attr(_cc_xml_t* ctx, const tchar_t* keyword, const tchar_t* fmt, ...);
/**
 * @brief Get XML Parse error message
 *
 * @return XML error message string
 */
_CC_API_PUBLIC(const tchar_t*) _cc_xml_error(void);
/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /*_C_CC_XML_H_INCLUDED_*/
