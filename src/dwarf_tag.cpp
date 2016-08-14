#include "dwarf_tag.h"
#include <stdint.h>
#include "dwarf.h"
#include "dwarf_attrib_table.h"
#include <stdio.h>
#include <stdexcept>


DwarfTag::DwarfTag(const uint8_t* const tagStart, const DwarfAttributeTable* attribTable, uint8_t adressSize) 
    : m_tagStart(tagStart)
    , m_attribTable(attribTable)
    , m_adressSize(adressSize)

    , m_attribEntryNumber(0)
    , m_infoPtrBase(nullptr)

    , m_hasChildren(false)
    , m_DW_TAG(0)
    , m_attribPtrBase(nullptr)

    , m_nextTagInfo(nullptr)
{
    m_infoPtrBase = readLEB128(m_tagStart, m_attribEntryNumber);
    //printf("DIE attrib entry: %d\n", m_attribEntryNumber);

    if (m_attribEntryNumber != 0)
    {
        auto attribEntryBase = m_attribTable->getCompilationUnitAbbrevEntry(m_attribEntryNumber);
        m_attribPtrBase = readLEB128(attribEntryBase, m_DW_TAG);
        m_hasChildren = *(m_attribPtrBase++);
        //printf(" hasChildren(%s) TAG: %s\n", m_hasChildren ? "X" : " ", DW_TAG_string(m_DW_TAG));
    }
}

uint32_t DwarfTag::getAttribNumber() const
{
    return m_attribEntryNumber;
}

uint32_t DwarfTag::getTag()
{
    return m_DW_TAG;
}

const unsigned char* const DwarfTag::getName() const
{
    if (isNullTag())
    {
        return nullptr;
    }

    const unsigned char* namePointer;
    auto callback = [&namePointer](uint32_t DW_AT, uint32_t DW_FORM, const uint8_t* info)
    {
        //printf("%s %s\n", DW_AT_string(DW_AT), DW_FORM_as_string(DW_FORM));
        if (DW_AT == DW_AT_name && DW_FORM == DW_FORM_string)
        {
            namePointer = info;
            return false;
        }
        return true;
    };
    readAttributes(callback);
    return namePointer;
}

void DwarfTag::readAttributes(std::function<bool(uint32_t attribName, uint32_t attribForm, const uint8_t *info)> callback) const
{
    if (isNullTag())
    {
        m_nextTagInfo = m_infoPtrBase;
        return;
    }

    uint32_t DW_AT;
    uint32_t DW_FORM;
    const uint8_t* attribPtr = m_attribPtrBase;
    const uint8_t* infoPtr = m_infoPtrBase;

    attribPtr = readLEB128(attribPtr, DW_AT);
    attribPtr = readLEB128(attribPtr, DW_FORM);
    while (DW_AT && DW_FORM)
    {
        if (DW_FORM == DW_FORM_indirect)
        {
            //printf("DW_FORM_indirect encountered!\n");
            infoPtr = readLEB128(infoPtr, DW_FORM);
        }
        //printf("%s(0x%X)\n  %s(0x%X)\n", DW_AT_string(DW_AT), DW_AT, DW_FORM_as_string(DW_FORM), DW_FORM);
        if (callback)
        {
            if (!callback(DW_AT, DW_FORM, infoPtr))
            {
                return;
            }
        }

        // Advance info pointer.
        switch (DW_FORM)
        {
        case DW_FORM_string:
            //printf("%s\n", infoPtr);
            while (*infoPtr != 0) ++infoPtr;
            ++infoPtr;
            break;
        case DW_FORM_strp:
            infoPtr += 4; // stringtable offset in .debug_str section.
            break;
        case DW_FORM_flag: // Tells if attribute is true or false (Ex DW_AT_external)
            infoPtr++;
            break;
        case DW_FORM_data1: // bytes of data!!
            ++infoPtr;
            break;
        case DW_FORM_data2:
            infoPtr += 2;
            break;
        case DW_FORM_data4:
            infoPtr += 4;
            break;
        case DW_FORM_data8:
            infoPtr += 8;
            break;
        case DW_FORM_udata:
        {
            uint32_t value;
            infoPtr = readLEB128(infoPtr, value);
            break;
        }
        case DW_FORM_sdata:
        {
            int32_t value;
            infoPtr = readLEB128(infoPtr, value);
            break;
        }

        case DW_FORM_addr:
            infoPtr += m_adressSize; // Refers to ANY DIE ENTRY in the binary
            break;
        case DW_FORM_ref1:
            infoPtr += 1; // cuHeader-relative offset to a DIE, 1 byte
            break;
        case DW_FORM_ref2:
            infoPtr += 2;
            break;
        case DW_FORM_ref4:
            infoPtr += 4;
            break;
        case DW_FORM_ref8:
            infoPtr += 8;
            break;
        case DW_FORM_ref_udata:
            uint64_t refAddr;
            infoPtr = readLEB128(infoPtr, refAddr);
            break;

        case DW_FORM_block1: // Simply blocks of data.
        {
            uint8_t size;
            size = *reinterpret_cast<const decltype(size)*>(infoPtr);
            infoPtr = infoPtr + sizeof(size) + size;
            break;
        }
        case DW_FORM_block2:
        {
            uint16_t size;
            size = *reinterpret_cast<const decltype(size)*>(infoPtr);
            infoPtr = infoPtr + sizeof(size) + size;
            break;
        }
        case DW_FORM_block4:
        {
            uint32_t size;
            size = *reinterpret_cast<const decltype(size)*>(infoPtr);
            infoPtr = infoPtr + sizeof(size) + size;
            break;
        }
        case DW_FORM_block:
        {
            uint32_t size;
            infoPtr = readLEB128(infoPtr, size);
            infoPtr += size;
            break;
        }

            throw std::logic_error("Dont get here; you missed a break!");
        default:
            throw std::runtime_error("unrecognized DW_FORM encountered");
        }

        attribPtr = readLEB128(attribPtr, DW_AT);
        attribPtr = readLEB128(attribPtr, DW_FORM);
    }
    m_nextTagInfo = infoPtr;
}


bool DwarfTag::isNullTag() const
{
    return m_attribEntryNumber == 0;
}
bool DwarfTag::hasChildren() const
{
    return m_hasChildren;
}

DwarfTag DwarfTag::getNextTag() const
{
    ensureAttributesRead();
    return DwarfTag(m_nextTagInfo, m_attribTable, m_adressSize);
}
const uint8_t* const DwarfTag::getNextTagPtr() const
{
    ensureAttributesRead();
    return m_nextTagInfo;
}


void DwarfTag::ensureAttributesRead() const
{
    if (m_nextTagInfo == nullptr)
    {
        //printf("looping attributes to obtain next DIE\n");
        readAttributes(nullptr);
    }
    if (m_nextTagInfo == nullptr)
    {
        throw std::logic_error("We can not have m_nextTagInfo == nullptr after having read the attribute... :S");
    }
}

bool DwarfTag::forAllTagsAndChildren(std::function<bool(const DwarfTag& tag, ptrdiff_t level)> callback) const
{

    if (!callback(*this, 0))
    {
        return false;
    }

    auto tag = *this;
    ptrdiff_t level = hasChildren();
    while (level > 0)
    {
        tag = tag.getNextTag();
        if (!callback(tag, level))
        {
            return false;
        }
        level += tag.hasChildren();
        level -= tag.isNullTag();
    }
    return true;
}







// DIE(1)
//  AttribEntry=1
//   AE(1):
//    Tag
//    HasChildren (x)
//    Attributes
//     nextSibling -> Die(1).nextSibling = attribValue
//
// DIE(2)
//  AttribEntry=2
//   AE(2):
//    Tag
//    HasChildren( )
//    Attributes
//
// DIE(3)
//  AttribEntry=0 -> 
//   
//
//
//
//
//

