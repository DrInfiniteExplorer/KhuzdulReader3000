#include "dwarf_attrib_table.h"
#include "dwarf.h"
#include <stdexcept>
#include <stdio.h>

DwarfAttributeTable::DwarfAttributeTable(const uint8_t* const attribTableStart, const uint8_t* const attribSectionEnd)
    : m_attribStart(attribTableStart)
    , m_attribSectionEnd(attribSectionEnd)
{
}

const uint8_t* const DwarfAttributeTable::getCompilationUnitAbbrevEntry(size_t entryNumber) const
{
    if (entryNumber == 0)
    {
        return nullptr;
    }
    //printf("Looking for DIE %d\n", entryNumber);
    auto abbrevStart = m_attribStart;

    auto abbrevPtr = abbrevStart;
    size_t readValue;
    while (abbrevPtr < m_attribSectionEnd)
    {
        auto asd = abbrevPtr;
        abbrevPtr = readLEB128(abbrevPtr, readValue);
        //printf("%p: A %d\n", asd - abbrevStart, readValue);
        if (readValue == entryNumber)
        {
            return abbrevPtr;
        }
        if (readValue == 0)
        {
            //printf("Attrib entry 0 encountered when looking for %d\n", entryNumber);
            break;
        }
        uint32_t DW_AT, DW_FORM;
        do{
            abbrevPtr = readLEB128(abbrevPtr, DW_AT);
            abbrevPtr = readLEB128(abbrevPtr, DW_FORM);
        } while (DW_AT || DW_FORM);
    }
    //printf("No matching attrib entry found\n");
    throw std::invalid_argument("Invalid compilation unit abbrev number");
}

