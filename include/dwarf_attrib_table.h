#pragma once
#include <stdint.h>
#include <stddef.h>

class DwarfAttributeTable
{
public:
    DwarfAttributeTable(const uint8_t* const attribTableStart, const uint8_t* const attribSectionEnd);

    // This returns a pointer to the start of the abbrev info data,
    //  ie points to after the LEB128-entry identifying the entry.
    const uint8_t* const getCompilationUnitAbbrevEntry(size_t entryNumber) const;

private:
    const uint8_t* const m_attribStart;
    const uint8_t* const m_attribSectionEnd;
    
};

