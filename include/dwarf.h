#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdexcept>
#include <type_traits>

#pragma pack(push,1)
struct CompilationUnitHeader
{
    uint32_t debug_info_contribution_length; //in bytes, contribution of this compilation unit (disregarding length field)
    uint16_t compilation_unit_dwarf_version; //2 for version 2
    uint32_t compilation_unit_debug_abbrev_offset;
    uint8_t  target_pointer_offset_size;
};
#pragma pop

template<int s, int t> struct check_equality {
    static_assert(s == t, "wrong size");
};
static check_equality<sizeof(uint32_t) * 8, 32> checkTheSize32;
static check_equality<sizeof(uint16_t) * 8, 16> checkTheSize16;
static check_equality<sizeof(uint8_t) * 8, 8> checkTheSize8;
static check_equality<sizeof(CompilationUnitHeader) * 8, 32 + 16 + 32 + 8> checkTheSize;

template <bool, int a, int b> struct selects {
    static const int val = a;
};
template <int a, int b> struct selects < false, a, b >{
    static const int val = b;
};

template <typename Type>
inline const uint8_t* readLEB128(const uint8_t* ptr, Type& result)
{
    if (*ptr & 0x80)
    {
        auto ret = readLEB128(ptr + 1, result);
        result = result << 7;
        result |= ((*ptr) & 0x7F);
        return ret;
    }
    result = *ptr;
    if (result & 0x40) {
        result |= (char)selects<std::is_signed<Type>::value, 0x80, 0x00>::val; // Extend sign
    }
    return ptr + 1;
}



#define DW_TAG_array_type 0x01
#define DW_TAG_class_type 0x02
#define DW_TAG_entry_point 0x03
#define DW_TAG_enumeration_type 0x04
#define DW_TAG_formal_parameter 0x05
#define DW_TAG_imported_declaration 0x08
#define DW_TAG_label 0x0a
#define DW_TAG_lexical_block 0x0b
#define DW_TAG_member 0x0d
#define DW_TAG_pointer_type 0x0f
#define DW_TAG_reference_type 0x10
#define DW_TAG_compile_unit 0x11
#define DW_TAG_string_type 0x12
#define DW_TAG_structure_type 0x13
#define DW_TAG_subroutine_type 0x15
#define DW_TAG_typedef 0x16
#define DW_TAG_union_type 0x17
#define DW_TAG_unspecified_parameters 0x18
#define DW_TAG_variant 0x19
#define DW_TAG_common_block 0x1a
#define DW_TAG_common_inclusion 0x1b
#define DW_TAG_inheritance 0x1c
#define DW_TAG_inlined_subroutine 0x1d
#define DW_TAG_module 0x1e
#define DW_TAG_ptr_to_member_type 0x1f
#define DW_TAG_set_type 0x20
#define DW_TAG_subrange_type 0x21
#define DW_TAG_with_stmt 0x22
#define DW_TAG_access_declaration 0x23
#define DW_TAG_base_type 0x24
#define DW_TAG_catch_block 0x25
#define DW_TAG_const_type 0x26
#define DW_TAG_constant 0x27
#define DW_TAG_enumerator 0x28
#define DW_TAG_file_type 0x29
#define DW_TAG_friend 0x2a
#define DW_TAG_namelist 0x2b
#define DW_TAG_namelist_item 0x2c
#define DW_TAG_packed_type 0x2d
#define DW_TAG_subprogram 0x2e
#define DW_TAG_template_type_param 0x2f
#define DW_TAG_template_value_param 0x30
#define DW_TAG_thrown_type 0x31
#define DW_TAG_try_block 0x32
#define DW_TAG_variant_part 0x33
#define DW_TAG_variable 0x34
#define DW_TAG_volatile_type 0x35
#define DW_TAG_lo_user 0x4080
#define DW_TAG_hi_user 0xffff

inline const char* const DW_TAG_string(uint64_t tag)
{
    if (tag >= DW_TAG_lo_user && tag <= DW_TAG_hi_user)
    { 
        return "In DW_TAG_lo_user-DW_TAG_hi_user range";
    }

    switch (tag)
    {
    case DW_TAG_array_type                 : return "DW_TAG_array_type";
    case DW_TAG_class_type                 : return "DW_TAG_class_type";
    case DW_TAG_entry_point                : return "DW_TAG_entry_point";
    case DW_TAG_enumeration_type           : return "DW_TAG_enumeration_type";
    case DW_TAG_formal_parameter           : return "DW_TAG_formal_parameter";
    case DW_TAG_imported_declaration       : return "DW_TAG_imported_declaration";
    case DW_TAG_label                      : return "DW_TAG_label";
    case DW_TAG_lexical_block              : return "DW_TAG_lexical_block";
    case DW_TAG_member                     : return "DW_TAG_member";
    case DW_TAG_pointer_type               : return "DW_TAG_pointer_type";
    case DW_TAG_reference_type             : return "DW_TAG_reference_type";
    case DW_TAG_compile_unit               : return "DW_TAG_compile_unit";
    case DW_TAG_string_type                : return "DW_TAG_string_type";
    case DW_TAG_structure_type             : return "DW_TAG_structure_type";
    case DW_TAG_subroutine_type            : return "DW_TAG_subroutine_type";
    case DW_TAG_typedef                    : return "DW_TAG_typedef";
    case DW_TAG_union_type                 : return "DW_TAG_union_type";
    case DW_TAG_unspecified_parameters     : return "DW_TAG_unspecified_parameters";
    case DW_TAG_variant                    : return "DW_TAG_variant";
    case DW_TAG_common_block               : return "DW_TAG_common_block";
    case DW_TAG_common_inclusion           : return "DW_TAG_common_inclusion";
    case DW_TAG_inheritance                : return "DW_TAG_inheritance";
    case DW_TAG_inlined_subroutine         : return "DW_TAG_inlined_subroutine";
    case DW_TAG_module                     : return "DW_TAG_module";
    case DW_TAG_ptr_to_member_type         : return "DW_TAG_ptr_to_member_type";
    case DW_TAG_set_type                   : return "DW_TAG_set_type";
    case DW_TAG_subrange_type              : return "DW_TAG_subrange_type";
    case DW_TAG_with_stmt                  : return "DW_TAG_with_stmt";
    case DW_TAG_access_declaration         : return "DW_TAG_access_declaration";
    case DW_TAG_base_type                  : return "DW_TAG_base_type";
    case DW_TAG_catch_block                : return "DW_TAG_catch_block";
    case DW_TAG_const_type                 : return "DW_TAG_const_type";
    case DW_TAG_constant                   : return "DW_TAG_constant";
    case DW_TAG_enumerator                 : return "DW_TAG_enumerator";
    case DW_TAG_file_type                  : return "DW_TAG_file_type";
    case DW_TAG_friend                     : return "DW_TAG_friend";
    case DW_TAG_namelist                   : return "DW_TAG_namelist";
    case DW_TAG_namelist_item              : return "DW_TAG_namelist_item";
    case DW_TAG_packed_type                : return "DW_TAG_packed_type";
    case DW_TAG_subprogram                 : return "DW_TAG_subprogram";
    case DW_TAG_template_type_param        : return "DW_TAG_template_type_param";
    case DW_TAG_template_value_param       : return "DW_TAG_template_value_param";
    case DW_TAG_thrown_type                : return "DW_TAG_thrown_type";
    case DW_TAG_try_block                  : return "DW_TAG_try_block";
    case DW_TAG_variant_part               : return "DW_TAG_variant_part";
    case DW_TAG_variable                   : return "DW_TAG_variable";
    case DW_TAG_volatile_type              : return "DW_TAG_volatile_type";
    default:
        return "<invalid DW_TAG-value>";
    }
}


#define DW_CHILDREN_no 0
#define DW_CHILDREN_yes 1
inline const char* const DW_CHILDREN_string(uint32_t val)
{
    switch (val)
    {
    case DW_CHILDREN_no:  return "DW_CHILDREN_no";
    case DW_CHILDREN_yes: return "DW_CHILDREN_yes";
    default:
        return "<invalid DW_CHILDREN-value>";
    }
}

#define DW_AT_sibling 0x01 //reference
#define DW_AT_location 0x02 //block, constant
#define DW_AT_name 0x03 //string
#define DW_AT_ordering 0x09 //constant
#define DW_AT_byte_size 0x0b //constant
#define DW_AT_bit_offset 0x0c //constant
#define DW_AT_bit_size 0x0d //constant
#define DW_AT_stmt_list 0x10 //constant
#define DW_AT_low_pc 0x11 //address
#define DW_AT_high_pc 0x12 //address
#define DW_AT_language 0x13 //constant
#define DW_AT_discr 0x15 //reference
#define DW_AT_discr_value 0x16 //constant
#define DW_AT_visibility 0x17 //constant
#define DW_AT_import 0x18 //reference
#define DW_AT_string_length 0x19 //block, constant
#define DW_AT_common_reference 0x1a //reference
#define DW_AT_comp_dir 0x1b //string
#define DW_AT_const_value 0x1c //string, constant, block
#define DW_AT_containing_type 0x1d //reference
#define DW_AT_default_value 0x1e //reference
#define DW_AT_inline 0x20 //constant
#define DW_AT_is_optional 0x21 //flag
#define DW_AT_lower_bound 0x22 //constant, reference
#define DW_AT_producer 0x25 //string
#define DW_AT_prototyped 0x27 //flag
#define DW_AT_return_addr 0x2a //block, constant
#define DW_AT_start_scope 0x2c //constant
#define DW_AT_stride_size 0x2e //constant
#define DW_AT_upper_bound 0x2f //constant, reference
#define DW_AT_abstract_origin 0x31 //reference
#define DW_AT_accessibility 0x32 //constant
#define DW_AT_address_class 0x33 //constant
#define DW_AT_artificial 0x34 //flag
#define DW_AT_base_types 0x35 //reference
#define DW_AT_calling_convention 0x36 //constant
#define DW_AT_count 0x37 //constant, reference
#define DW_AT_data_member_location 0x38 //block, reference
#define DW_AT_decl_column 0x39 //constant
#define DW_AT_decl_file 0x3a //constant
#define DW_AT_decl_line 0x3b //constant
#define DW_AT_declaration 0x3c //flag
#define DW_AT_discr_list 0x3d //block
#define DW_AT_encoding 0x3e //constant
#define DW_AT_external 0x3f //flag
#define DW_AT_frame_base 0x40 //block, constant
#define DW_AT_friend 0x41 //reference
#define DW_AT_identifier_case 0x42 //constant
#define DW_AT_macro_info 0x43 //constant
#define DW_AT_namelist_item 0x44 //block
#define DW_AT_priority 0x45 //reference
#define DW_AT_segment 0x46 //block, constant
#define DW_AT_specification 0x47 //reference
#define DW_AT_static_link 0x48 //block, constant
#define DW_AT_type 0x49 //reference
#define DW_AT_use_location 0x4a //block, constant
#define DW_AT_variable_parameter 0x4b //flag
#define DW_AT_virtuality 0x4c //constant
#define DW_AT_vtable_elem_location 0x4d //block, reference
#define DW_AT_lo_user 0x2000 //—
#define DW_AT_hi_user 0x3fff //—

inline const char* const DW_AT_string(uint32_t val)
{
    if (DW_AT_lo_user <= val && val <= DW_AT_hi_user)
    {
        return "In DW_AT_lo_user-DW_AT_hi_user range";
    }
    switch (val)
    {
    case DW_AT_sibling               : return "DW_AT_sibling";
    case DW_AT_location              : return "DW_AT_location";
    case DW_AT_name                  : return "DW_AT_name";
    case DW_AT_ordering              : return "DW_AT_ordering";
    case DW_AT_byte_size             : return "DW_AT_byte_size";
    case DW_AT_bit_offset            : return "DW_AT_bit_offset";
    case DW_AT_bit_size              : return "DW_AT_bit_size";
    case DW_AT_stmt_list             : return "DW_AT_stmt_list";
    case DW_AT_low_pc                : return "DW_AT_low_pc";
    case DW_AT_high_pc               : return "DW_AT_high_pc";
    case DW_AT_language              : return "DW_AT_language";
    case DW_AT_discr                 : return "DW_AT_discr";
    case DW_AT_discr_value           : return "DW_AT_discr_value";
    case DW_AT_visibility            : return "DW_AT_visibility";
    case DW_AT_import                : return "DW_AT_import";
    case DW_AT_string_length         : return "DW_AT_string_length";
    case DW_AT_common_reference      : return "DW_AT_common_reference";
    case DW_AT_comp_dir              : return "DW_AT_comp_dir";
    case DW_AT_const_value           : return "DW_AT_const_value";
    case DW_AT_containing_type       : return "DW_AT_containing_type";
    case DW_AT_default_value         : return "DW_AT_default_value";
    case DW_AT_inline                : return "DW_AT_inline";
    case DW_AT_is_optional           : return "DW_AT_is_optional";
    case DW_AT_lower_bound           : return "DW_AT_lower_bound";
    case DW_AT_producer              : return "DW_AT_producer";
    case DW_AT_prototyped            : return "DW_AT_prototyped";
    case DW_AT_return_addr           : return "DW_AT_return_addr";
    case DW_AT_start_scope           : return "DW_AT_start_scope";
    case DW_AT_stride_size           : return "DW_AT_stride_size";
    case DW_AT_upper_bound           : return "DW_AT_upper_bound";
    case DW_AT_abstract_origin       : return "DW_AT_abstract_origin";
    case DW_AT_accessibility         : return "DW_AT_accessibility";
    case DW_AT_address_class         : return "DW_AT_address_class";
    case DW_AT_artificial            : return "DW_AT_artificial";
    case DW_AT_base_types            : return "DW_AT_base_types";
    case DW_AT_calling_convention    : return "DW_AT_calling_convention";
    case DW_AT_count                 : return "DW_AT_count";
    case DW_AT_data_member_location  : return "DW_AT_data_member_location";
    case DW_AT_decl_column           : return "DW_AT_decl_column";
    case DW_AT_decl_file             : return "DW_AT_decl_file";
    case DW_AT_decl_line             : return "DW_AT_decl_line";
    case DW_AT_declaration           : return "DW_AT_declaration";
    case DW_AT_discr_list            : return "DW_AT_discr_list";
    case DW_AT_encoding              : return "DW_AT_encoding";
    case DW_AT_external              : return "DW_AT_external";
    case DW_AT_frame_base            : return "DW_AT_frame_base";
    case DW_AT_friend                : return "DW_AT_friend";
    case DW_AT_identifier_case       : return "DW_AT_identifier_case";
    case DW_AT_macro_info            : return "DW_AT_macro_info";
    case DW_AT_namelist_item         : return "DW_AT_namelist_item";
    case DW_AT_priority              : return "DW_AT_priority";
    case DW_AT_segment               : return "DW_AT_segment";
    case DW_AT_specification         : return "DW_AT_specification";
    case DW_AT_static_link           : return "DW_AT_static_link";
    case DW_AT_type                  : return "DW_AT_type";
    case DW_AT_use_location          : return "DW_AT_use_location";
    case DW_AT_variable_parameter    : return "DW_AT_variable_parameter";
    case DW_AT_virtuality            : return "DW_AT_virtuality";
    case DW_AT_vtable_elem_location  : return "DW_AT_vtable_elem_location";
    default:
        return "<invalid DW_AT-value>";
    }
}

#define DW_FORM_addr 0x01 //address
#define DW_FORM_block2 0x03 //block
#define DW_FORM_block4 0x04 //block
#define DW_FORM_data2 0x05 //constant
#define DW_FORM_data4 0x06 //constant
#define DW_FORM_data8 0x07 //constant
#define DW_FORM_string 0x08 //string
#define DW_FORM_block 0x09 //block
#define DW_FORM_block1 0x0a //block
#define DW_FORM_data1 0x0b //constant
#define DW_FORM_flag 0x0c //flag
#define DW_FORM_sdata 0x0d //constant
#define DW_FORM_strp 0x0e //string
#define DW_FORM_udata 0x0f //constant
#define DW_FORM_ref_addr 0x10 //reference
#define DW_FORM_ref1 0x11 //reference
#define DW_FORM_ref2 0x12 //reference
#define DW_FORM_ref4 0x13 //reference
#define DW_FORM_ref8 0x14 //reference
#define DW_FORM_ref_udata 0x15 //reference
#define DW_FORM_indirect 0x16 //(see section 7.5.3)

inline const char* const DW_FORM_as_string(uint32_t val)
{
    switch (val)
    {
    case DW_FORM_addr      : return "DW_FORM_addr";
    case DW_FORM_block2    : return "DW_FORM_block2";
    case DW_FORM_block4    : return "DW_FORM_block4";
    case DW_FORM_data2     : return "DW_FORM_data2";
    case DW_FORM_data4     : return "DW_FORM_data4";
    case DW_FORM_data8     : return "DW_FORM_data8";
    case DW_FORM_string    : return "DW_FORM_string";
    case DW_FORM_block     : return "DW_FORM_block";
    case DW_FORM_block1    : return "DW_FORM_block1";
    case DW_FORM_data1     : return "DW_FORM_data1";
    case DW_FORM_flag      : return "DW_FORM_flag";
    case DW_FORM_sdata     : return "DW_FORM_sdata";
    case DW_FORM_strp      : return "DW_FORM_strp";
    case DW_FORM_udata     : return "DW_FORM_udata";
    case DW_FORM_ref_addr  : return "DW_FORM_ref_addr";
    case DW_FORM_ref1      : return "DW_FORM_ref1";
    case DW_FORM_ref2      : return "DW_FORM_ref2";
    case DW_FORM_ref4      : return "DW_FORM_ref4";
    case DW_FORM_ref8      : return "DW_FORM_ref8";
    case DW_FORM_ref_udata : return "DW_FORM_ref_udata";
    case DW_FORM_indirect  : return "DW_FORM_indirect";
    default:
        return "<invalid DW_FORM-value>";
    }
}

template <uint32_t expected>
inline void expectForm(uint32_t DW_FORM)
{
    if (expected != DW_FORM)
    {
        throw std::invalid_argument("Got wrong sort of DW_FORM");
    }
}

// NAME VALUE OPERANDS (NOTES)
#define DW_OP_addr 0x03 //1 constant address(size target specific)
#define DW_OP_deref 0x06 //0
#define DW_OP_const1u 0x08 //1 1 - byte constant
#define DW_OP_const1s 0x09 //1 1 - byte constant
#define DW_OP_const2u 0x0a //1 2 - byte constant
#define DW_OP_const2s 0x0b //1 2 - byte constant
#define DW_OP_const4u 0x0c //1 4 - byte constant
#define DW_OP_const4s 0x0d //1 4 - byte constant
#define DW_OP_const8u 0x0e //1 8 - byte constant
#define DW_OP_const8s 0x0f //1 8 - byte constant
#define DW_OP_constu 0x10 //1 ULEB128 constant
#define DW_OP_consts 0x11 //1 SLEB128 constant
#define DW_OP_dup 0x12 //0
#define DW_OP_drop 0x13 //0
#define DW_OP_over 0x14 //0
#define DW_OP_pick 0x15 //1 1 - byte stack index
#define DW_OP_swap 0x16 //0
#define DW_OP_rot 0x17 //0
#define DW_OP_xderef 0x18 //0
#define DW_OP_abs 0x19 //0
#define DW_OP_and 0x1a //0
#define DW_OP_div 0x1b //0
#define DW_OP_minus 0x1c //0
#define DW_OP_mod 0x1d //0
#define DW_OP_mul 0x1e //0
#define DW_OP_neg 0x1f //0
#define DW_OP_not 0x20 //0
#define DW_OP_or 0x21 //0
#define DW_OP_plus 0x22 //0
#define DW_OP_plus_uconst 0x23 //1 ULEB128 addend
#define DW_OP_shl 0x24 //0
#define DW_OP_shr 0x25 //0
#define DW_OP_shra 0x26 //0
#define DW_OP_xor 0x27 //0
#define DW_OP_skip 0x2f //1 signed 2 - byte constant
#define DW_OP_bra 0x28 //1 signed 2 - byte constant
#define DW_OP_eq 0x29 //0
#define DW_OP_ge 0x2a //0
#define DW_OP_gt 0x2b //0
#define DW_OP_le 0x2c //0
#define DW_OP_lt 0x2d //0
#define DW_OP_ne 0x2e //0
#define DW_OP_lit0 0x30 //0 literals 0..31 = (DW_OP_LIT0ïliteral)
#define DW_OP_lit1 0x31 //0
#define DW_OP_lit31 0x4f //0
#define DW_OP_reg0 0x50 //0 reg 0..31 = (DW_OP_REG0ïregnum)
#define DW_OP_reg1 0x51 //0
#define DW_OP_reg31 0x6f //0
#define DW_OP_breg0 0x70 //1 SLEB128 offset
#define DW_OP_breg1 0x71 //1 base reg 0..31 = (DW_OP_BREG0ïregnum)
#define DW_OP_breg31 0x8f //1
#define DW_OP_regx 0x90 //1 ULEB128 register
#define DW_OP_fbreg 0x91 //1 SLEB128 offset
#define DW_OP_bregx 0x92 //2 ULEB128 register followed by SLEB128 offset
#define DW_OP_piece 0x93 //1 ULEB128 size of piece addressed
#define DW_OP_deref_size 0x94 //1 1 - byte size of data retrieved
#define DW_OP_xderef_size 0x95 //1 1 - byte size of data retrieved
#define DW_OP_nop 0x96 //0
#define DW_OP_lo_user 0xe0 //
#define DW_OP_hi_user 0xff //

#define DW_ATE_address 0x1
#define DW_ATE_boolean 0x2
#define DW_ATE_complex_float 0x3
#define DW_ATE_float 0x4
#define DW_ATE_signed 0x5
#define DW_ATE_signed_char 0x6
#define DW_ATE_unsigned 0x7
#define DW_ATE_unsigned_char 0x8
#define DW_ATE_lo_user 0x80
#define DW_ATE_hi_user 0xff


#define DW_ACCESS_public 1
#define DW_ACCESS_protected 2
#define DW_ACCESS_private 3

#define DW_VIS_local 1
#define DW_VIS_exported 2
#define DW_VIS_qualified 3 

#define DW_VIRTUALITY_none 0
#define DW_VIRTUALITY_virtual 1
#define DW_VIRTUALITY_pure_virtual 2

#define DW_LANG_C89 0x0001
#define DW_LANG_C 0x0002
#define DW_LANG_Ada83 0x0003
#define DW_LANG_C_plus_plus 0x0004
#define DW_LANG_Cobol74 0x0005
#define DW_LANG_Cobol85 0x0006
#define DW_LANG_Fortran77 0x0007
#define DW_LANG_Fortran90 0x0008
#define DW_LANG_Pascal83 0x0009
#define DW_LANG_Modula2 0x000a
#define DW_LANG_lo_user 0x8000
#define DW_LANG_hi_user 0xffff

inline const char* const DW_LANG_string(uint32_t val)
{
    if (DW_LANG_lo_user <= val && val <= DW_LANG_hi_user)
    {
        return "In DW_LANG_lo_user-DW_LANG_hi_user range";
    }   
    
    switch (val)
    {
    case DW_LANG_C89         : return "DW_LANG_C89";
    case DW_LANG_C           : return "DW_LANG_C";
    case DW_LANG_Ada83       : return "DW_LANG_Ada83";
    case DW_LANG_C_plus_plus : return "DW_LANG_C_plus_plus";
    case DW_LANG_Cobol74     : return "DW_LANG_Cobol74";
    case DW_LANG_Cobol85     : return "DW_LANG_Cobol85";
    case DW_LANG_Fortran77   : return "DW_LANG_Fortran77";
    case DW_LANG_Fortran90   : return "DW_LANG_Fortran90";
    case DW_LANG_Pascal83    : return "DW_LANG_Pascal83";
    case DW_LANG_Modula2     : return "DW_LANG_Modula2";

    default:
        return "<invalid DW_LANG-value>";
    }
}

#define DW_ID_case_sensitive 0
#define DW_ID_up_case 1
#define DW_ID_down_case 2
#define DW_ID_case_insensitive 3

#define DW_CC_normal 0x1
#define DW_CC_program 0x2
#define DW_CC_nocall 0x3
#define DW_CC_lo_user 0x40
#define DW_CC_hi_user 0xff

#define DW_INL_not_inlined 0
#define DW_INL_inlined 1
#define DW_INL_declared_not_inlined 2
#define DW_INL_declared_inlined 3

#define DW_ORD_row_major 0
#define DW_ORD_col_major 1

#define DW_DSC_label 0
#define DW_DSC_range 1

#define DW_LNS_copy 1
#define DW_LNS_advance_pc 2
#define DW_LNS_advance_line 3
#define DW_LNS_set_file 4
#define DW_LNS_set_column 5
#define DW_LNS_negate_stmt 6
#define DW_LNS_set_basic_block 7
#define DW_LNS_const_add_pc 8
#define DW_LNS_fixed_advance_pc 9

#define DW_LNE_end_sequence 1
#define DW_LNE_set_address 2
#define DW_LNE_define_file 3

#define DW_MACINFO_define 1
#define DW_MACINFO_undef 2
#define DW_MACINFO_start_file 3
#define DW_MACINFO_end_file 4
#define DW_MACINFO_vendor_ext 255

//__ I_n_s_tr_u_c_ti_o_n_ ______________________H_i_g_h_ 2_ _B_i_ts_ ___L_o_w__ 6_ _B_i_ts_ ___O__p_er_a_n_d_ 1_ __________O_p_e_r_a_n_d_ 2________
//DW_CFA_advance_loc 0x1 delta
//DW_CFA_offset 0x2 register ULEB128 offset
//DW_CFA_restore 0x3 register
//DW_CFA_set_loc 0 0x01 address
//DW_CFA_advance_loc1 0 0x02 1 - byte delta
//DW_CFA_advance_loc2 0 0x03 2 - byte delta
//DW_CFA_advance_loc4 0 0x04 4 - byte delta
//DW_CFA_offset_extended 0 0x05 ULEB128 register ULEB128 offset
//DW_CFA_restore_extended 0 0x06 ULEB128 register
//DW_CFA_undefined 0 0x07 ULEB128 register
//DW_CFA_same_value 0 0x08 ULEB128 register
//DW_CFA_register 0 0x09 ULEB128 register ULEB128 register
//DW_CFA_remember_state 0 0x0a
//DW_CFA_restore_state 0 0x0b
//DW_CFA_def_cfa 0 0x0c ULEB128 register ULEB128 offset
//DW_CFA_def_cfa_register 0 0x0d ULEB128 register
//DW_CFA_def_cfa_offset 0 0x0e ULEB128 offset
//DW_CFA_nop 0 0
//DW_CFA_lo_user 0 0x1c
//DW_CFA_hi_user 0 0x3f






