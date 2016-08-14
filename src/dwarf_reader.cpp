#include "dwarf_reader.h"
#include "dwarf.h"
#include <stdio.h>
#include "dwarf_attrib_table.h"




DwarfReader::DwarfReader(const std::shared_ptr<Image> &sourceImage)
{
    m_image = sourceImage;

    if (!m_image->getSectionPointer(".debug_info", m_debug_info, m_debug_info_size))
    {
        throw std::invalid_argument("The supplied image lacks a .debug_info-section");
    }

    if (!m_image->getSectionPointer(".debug_abbrev", m_debug_abbrev, m_debug_abbrev_size))
    {
        throw std::invalid_argument("The supplied image lacks a .debug_abbrev-section");
    }

    populateCompilationUnitHeaders();
}

void print(CompilationUnitHeader* cu)
{
    printf(
        "Compilation Unit %p\n"
        " CompilationUnitLength %d\n"
        " Dwarf Version %d\n"
        " Attrib table offset %d\n"
        " Pointer size %d\n"
        , cu
        , cu->debug_info_contribution_length
        , cu->compilation_unit_dwarf_version
        , cu->compilation_unit_debug_abbrev_offset
        , cu->target_pointer_offset_size);
}

DwarfReader::~DwarfReader()
{
    for (auto& pair : m_attributeTables)
    {
        auto attribTable = pair.second;
        delete attribTable;
    }
}

const CompilationUnitHeader* const DwarfReader::getCompilationUnitHeader(size_t index) const
{
    if (m_compilationUnitHeaders.size() <= index)
    {
        throw std::invalid_argument("Invalid index for compilation unit header");
    }
    return m_compilationUnitHeaders[index];
}

DwarfCompilationUnit DwarfReader::getCompilationUnit(size_t index) const
{
    auto cuHeader = getCompilationUnitHeader(index);

    auto attribBase = m_debug_abbrev + cuHeader->compilation_unit_debug_abbrev_offset;

    DwarfAttributeTable* attribTable;
    auto found = m_attributeTables.find(attribBase);
    if (found == m_attributeTables.end())
    {
        attribTable = new DwarfAttributeTable(attribBase, m_debug_abbrev + m_debug_abbrev_size);
    }
    else
    {
        attribTable = found->second;
    }

    return DwarfCompilationUnit(this, attribTable, cuHeader);
}

size_t DwarfReader::getCompilationUnitCount() const
{
    return m_compilationUnitHeaders.size();
}

void DwarfReader::populateCompilationUnitHeaders()
{
    size_t offset = 0;
    size_t cuCount = 0;
    // Count to preallocate
    while (offset < m_debug_info_size)
    {
        auto cuHeader = reinterpret_cast<CompilationUnitHeader*>(m_debug_info + offset);
        offset += sizeof(cuHeader->debug_info_contribution_length) + cuHeader->debug_info_contribution_length;
        cuCount++;
    }

    // Allocate and fill
    m_compilationUnitHeaders.reserve(cuCount);
    offset = 0;
    while (offset < m_debug_info_size)
    {
        auto cuHeader = reinterpret_cast<CompilationUnitHeader*>(m_debug_info + offset);
        offset += sizeof(cuHeader->debug_info_contribution_length) + cuHeader->debug_info_contribution_length;
        m_compilationUnitHeaders.push_back(cuHeader);
    }
}


