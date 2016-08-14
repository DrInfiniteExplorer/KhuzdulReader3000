#pragma once
#include <memory>
#include "image.h"
#include <vector>
#include "dwarf.h"
#include "dwarf_compilation_unit.h"
#include <map>

class DwarfAttributeTable;

class DwarfReader
{
public:
    DwarfReader(const std::shared_ptr<Image> &sourceImage);

    virtual ~DwarfReader();

    const CompilationUnitHeader* const getCompilationUnitHeader(size_t index) const;

    DwarfCompilationUnit getCompilationUnit(size_t index) const;
    
    size_t getCompilationUnitCount() const;

private:
    std::shared_ptr<Image> m_image;

    uint8_t* m_debug_info;
    size_t   m_debug_info_size;

    uint8_t* m_debug_abbrev;
    size_t   m_debug_abbrev_size;

    void populateCompilationUnitHeaders();
    std::vector<const CompilationUnitHeader*> m_compilationUnitHeaders;
    std::map<uint8_t*, DwarfAttributeTable*> m_attributeTables;
};


