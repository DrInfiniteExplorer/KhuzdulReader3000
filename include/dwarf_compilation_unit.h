#pragma once
#include "dwarf.h"
#include "dwarf_tag.h"

class DwarfReader;
class DwarfAttributeTable;

class DwarfCompilationUnit
{
public:
    DwarfCompilationUnit(const DwarfReader* const dwarfReader, const DwarfAttributeTable* const attribTable, const CompilationUnitHeader* const cuHeader);


    const uint8_t* const getCompilationUnitEndPtr(const CompilationUnitHeader* const cuHeader);

    DwarfTag getFirstTag() const;

    const uint8_t* const getName() const;
    const uint8_t* const getProducer() const;
    uint8_t getLanguage() const;

    void print(const CompilationUnitHeader* const cuHeader) const;

private:

    const CompilationUnitHeader* const m_cuHeader;
    const DwarfAttributeTable* const m_attribTable;
    const DwarfReader* const m_dwarfReader;
    const uint8_t* m_name;
    const uint8_t* m_producer;
    uint8_t m_language;
    bool populateInfo(uint32_t attribName, uint32_t attribForm, const uint8_t *info);
};

