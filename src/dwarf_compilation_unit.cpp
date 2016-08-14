#include "dwarf_compilation_unit.h"
#include <stdexcept>
#include <stdio.h>
#include <cstdlib>

//template <typename Class, typename Ret, typename... Args>
//std::function<Ret(Args...)> Bind(Class* instance, Ret(Class::*func)(Args...))
//{
//    return [instance, func](Args&&... args) {
//        return (instance->*func)(std::forward<Args>(args)...);
//    };
//}

DwarfCompilationUnit::DwarfCompilationUnit(const DwarfReader* const dwarfReader, const DwarfAttributeTable* const attribTable, const CompilationUnitHeader* const cuHeader) 
    : m_cuHeader(cuHeader)
    , m_attribTable(attribTable)
    , m_dwarfReader(dwarfReader)
    , m_name(nullptr)
    , m_producer(nullptr)
    , m_language(0)
{
    //auto asd = std::bind(&DwarfCompilationUnit::getFirstTag, this);
    //auto tag = asd();

    auto tag = getFirstTag();
    auto tagType = tag.getTag();
    if (tagType != DW_TAG_compile_unit)
    {
        throw std::logic_error("Expected a DW_TAG_COMPILE_UNIT...");
    }

    auto asd = std::bind(&DwarfCompilationUnit::populateInfo, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
    tag.readAttributes(asd);

    print(cuHeader);

    //std::function<DwarfTag()> asd = &getFirstTag;
}

DwarfTag DwarfCompilationUnit::getFirstTag() const
{
    auto cuBase = reinterpret_cast<const uint8_t*>(m_cuHeader+1);

    return DwarfTag(cuBase, m_attribTable, m_cuHeader->target_pointer_offset_size);
}

const uint8_t* const DwarfCompilationUnit::getName() const
{
    return m_name;
}

const uint8_t* const DwarfCompilationUnit::getProducer() const
{
    return m_producer;
}

uint8_t DwarfCompilationUnit::getLanguage() const
{
    return m_language;
}

const uint8_t* const DwarfCompilationUnit::getCompilationUnitEndPtr(const CompilationUnitHeader* const cuHeader)
{
    return reinterpret_cast<const uint8_t* const>(cuHeader)+cuHeader->debug_info_contribution_length + 4;
}

void DwarfCompilationUnit::print(const CompilationUnitHeader* const cuHeader) const
{
    printf("Compilation unit %p\n", cuHeader);
    printf(" Language: %s\n", DW_LANG_string(m_language));
    printf(" Producer: %s\n", m_producer);
    printf(" Source: %s\n", m_name);
}

bool DwarfCompilationUnit::populateInfo(uint32_t attribName, uint32_t attribForm, const uint8_t *info)
{
    switch (attribName)
    {
    case DW_AT_name:
        expectForm<DW_FORM_string>(attribForm);
        m_name = info;
        break;
    case DW_AT_producer:
        expectForm<DW_FORM_string>(attribForm);
        m_producer = info;
        break;
    case DW_AT_language:
        expectForm<DW_FORM_data1>(attribForm);
        m_language = *info;
        break;

    default:
        break;
    }
    return true;
}
