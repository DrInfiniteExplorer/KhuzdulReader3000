#pragma once
#include "dwarf.h"
#include <functional>


// Abbreviation/Attribute/Tag layout
// LEB128 abbreviation-id
// LEB128 Tag (DW_TAG_*)
// 1 byte HasChild (DW_CHILDREN_[yes|no]
// Repeat:
//   LEB128 Attribute name
//   LEB128 Attribute form (type/encoding)
// LEB128(0)
// LEB128(0)

class DwarfAttributeTable;

class DwarfTag
{
public:
    DwarfTag(const uint8_t* const tagStart, const DwarfAttributeTable* attribTable, uint8_t adressSize);

    uint32_t getAttribNumber() const;
    uint32_t getTag();
    const unsigned char* const getName() const;

    void readAttributes(std::function<bool(uint32_t attribName, uint32_t attribForm, const uint8_t *info)> callback) const;

    bool isNullTag() const;
    bool hasChildren() const;

    bool forAllTagsAndChildren(std::function<bool(const DwarfTag& tag, ptrdiff_t level)>) const;

    DwarfTag getNextTag() const;
    const uint8_t* const getNextTagPtr() const;
private:
    void ensureAttributesRead() const;
    const uint8_t* m_tagStart;
    const DwarfAttributeTable* m_attribTable;
    uint8_t m_adressSize;


    uint32_t m_attribEntryNumber;
    const uint8_t* m_infoPtrBase;

    uint8_t m_hasChildren;
    uint32_t m_DW_TAG;
    const uint8_t* m_attribPtrBase;

    mutable const uint8_t* m_nextTagInfo;
};







