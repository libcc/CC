#include <libcc/string.h>
#include <libcc/platform.h>
#include <assert.h>
#include <stdio.h>

/**
 * Test that the GBK to Unicode table is properly initialized
 */
void test_gbk_table_initialization() {
    // Verify the table exists and has the correct size
    assert(gs_gbk2uni_table != NULL);
    assert(sizeof(gs_gbk2uni_table)/sizeof(gs_gbk2uni_table[0]) == 24066);
    
    // Check first few entries
    assert(gs_gbk2uni_table[0] == 0x4E02);
    assert(gs_gbk2uni_table[1] == 0x4E04);
    assert(gs_gbk2uni_table[2] == 0x4E05);
    
    // Check some middle entries
    assert(gs_gbk2uni_table[100] == 0x50A0);
    assert(gs_gbk2uni_table[1000] == 0x7DF4);
    
    // Check last entry
    assert(gs_gbk2uni_table[24065] == 0xE7FF);
    
    printf("GBK table initialization test passed!\n");
}

/**
 * Test valid GBK to Unicode conversions
 */
void test_valid_gbk_conversions() {
    // Test some sample conversions
    assert(_cc_gbk_to_unicode(0xB0A1) == 0x4E02); // First entry
    assert(_cc_gbk_to_unicode(0xB0A2) == 0x4E04);
    assert(_cc_gbk_to_unicode(0xB0A3) == 0x4E05);
    
    // Test some common Chinese characters
    assert(_cc_gbk_to_unicode(0xBBD5) == 0x5B57); // 字
    assert(_cc_gbk_to_unicode(0xD3DA) == 0x6C49); // 汉
    
    printf("Valid GBK conversion test passed!\n");
}

/**
 * Test invalid GBK characters
 */
void test_invalid_gbk_characters() {
    // Test characters outside GBK range
    assert(_cc_gbk_to_unicode(0x00) == 0x003F);   // Below GBK range
    assert(_cc_gbk_to_unicode(0xA0) == 0x003F);    // Below GBK range
    assert(_cc_gbk_to_unicode(0xFFFF) == 0x003F); // Above GBK range
    
    // Test undefined GBK codes
    assert(_cc_gbk_to_unicode(0xA8A1) == 0x003F); // Undefined in GBK
    
    printf("Invalid GBK character test passed!\n");
}

/**
 * Test replacement character handling
 */
void test_replacement_character() {
    // Verify the replacement character is returned for invalid/undefined GBK
    assert(_cc_gbk_to_unicode(0x0000) == 0x003F);
    assert(_cc_gbk_to_unicode(0xFFFF) == 0x003F);
    
    // Check explicit replacement characters in the table
    assert(gs_gbk2uni_table[15] == 0x003F); // Sample replacement in table
    
    printf("Replacement character test passed!\n");
}

/**
 * Test boundary conditions
 */
void test_boundary_conditions() {
    // First and last defined GBK characters
    assert(_cc_gbk_to_unicode(0xA1A1) != 0x003F); // First GBK char
    assert(_cc_gbk_to_unicode(0xFEFE) != 0x003F); // Last GBK char
    
    // Just outside boundaries
    assert(_cc_gbk_to_unicode(0xA1A0) == 0x003F); // Just below
    assert(_cc_gbk_to_unicode(0xFEFF) == 0x003F); // Just above
    
    printf("Boundary condition test passed!\n");
}

int main() {
    test_gbk_table_initialization();
    test_valid_gbk_conversions();
    test_invalid_gbk_characters();
    test_replacement_character();
    test_boundary_conditions();
    
    printf("All GBK tests passed!\n");
    return 0;
}