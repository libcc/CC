#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libcc/widgets/xml.h>

const _cc_String_t xml_content = _cc_String("<?xml version=\"1.0\" encoding=\"UTF-8\"?><root attr=\"value\"><code>200</code><data><c>Hello World</c><e>1728576000000</e><s>vIOiQyA7giWK</s><t>154ff80489dacae94c45247235cd8083</t><u>js/dfxaf3-ba348b9e.js</u><a>1</a><a>2</a><a>3</a><a>4</a><a>5</a><a>6</a><a>7</a><a>8</a><a>9</a><a>0</a></data><id>5bb5f254-36a8-4267-94fb-22287d0d2477</id><message><![CDATA[This is <b>bold</b> text & more!]]></message></root>");
/* Helper function to print test results */
void print_test_result(const char *test_name, int passed) {
    printf("%s: %s\n", test_name, passed ? "PASSED" : "FAILED");
}

/* Test cases */
void test_xml_new_element() {
    _cc_xml_t *xml = _cc_new_xml_element(_CC_XML_CHILD_);
    assert(xml != NULL);
    assert(xml->type == _CC_XML_CHILD_);
    _cc_free_xml(xml);
    print_test_result(__func__, 1);
}

void test_xml_from_file() {
    const tchar_t *file_name = "test.xml";
    _cc_xml_t *xml = _cc_xml_from_file(file_name);
    assert(xml != NULL);
    _cc_free_xml(xml);
    print_test_result(__func__, 1);
}

void test_xml_parse() {
    _cc_xml_t *xml = _cc_xml_parse(xml_content.data, xml_content.length);
    assert(xml != NULL);
    _cc_free_xml(xml);
    print_test_result(__func__, 1);
}

void test_xml_element_append() {
    _cc_xml_t *parent = _cc_new_xml_element(_CC_XML_CHILD_);
    _cc_xml_t *child = _cc_new_xml_element(_CC_XML_CHILD_);
    assert(_cc_xml_element_append(parent, child));
    _cc_free_xml(parent);
    print_test_result(__func__, 1);
}

void test_xml_element_find() {
    _cc_xml_t *xml = _cc_xml_parse(xml_content.data, xml_content.length);
    _cc_xml_t *found = _cc_xml_element_find(xml, "message");
    assert(found != NULL);
    _cc_free_xml(xml);
    print_test_result(__func__, 1);
}

void test_xml_element_text() {
    _cc_xml_t *xml = _cc_xml_parse(xml_content.data, xml_content.length);
    _cc_xml_t *found = _cc_xml_element_find(xml, "message");
    const tchar_t *text = _cc_xml_element_text(found);
    assert(text != NULL);
    _cc_free_xml(xml);
    print_test_result(__func__, 1);
}

void test_xml_element_attr_find() {
    _cc_xml_t *xml = _cc_xml_parse(xml_content.data, xml_content.length);
    const tchar_t *attr = _cc_xml_element_attr_find(xml, "attr");
    assert(attr != NULL);
    _cc_free_xml(xml);
    print_test_result(__func__, 1);
}

void test_xml_element_set_attr() {
    _cc_xml_t *xml = _cc_new_xml_element(_CC_XML_CHILD_);
    assert(_cc_xml_element_set_attr(xml, "attr", "value"));
    _cc_free_xml(xml);
    print_test_result(__func__, 1);
}

void test_xml_error() {
    const tchar_t *error = _cc_xml_error();
    assert(error != NULL);
    print_test_result(__func__, 1);
}

int main() {
    test_xml_new_element();
    test_xml_from_file();
    test_xml_parse();
    test_xml_element_append();
    test_xml_element_find();
    test_xml_element_text();
    test_xml_element_attr_find();
    test_xml_element_set_attr();
    test_xml_error();
    return 0;
}