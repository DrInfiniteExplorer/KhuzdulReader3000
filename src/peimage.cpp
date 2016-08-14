#include "peimage.h"
#include <stdexcept>
#include <stdio.h>

PEImage::PEImage()
    : Image()
{

}

void PEImage::dispose()
{
    throw std::logic_error("The method or operation is not implemented.");
}

void PEImage::loadImage(void* imageData, size_t virtualAddress)
{
    m_imagePtr = static_cast<uint8_t*>(imageData);
    m_virtualBase = virtualAddress;

    m_dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(m_imagePtr);
    if (m_dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
    {
        throw std::invalid_argument("Invalid dos header");
    }

    m_ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(m_imagePtr + m_dosHeader->e_lfanew);
    if (m_ntHeader->Signature != IMAGE_NT_SIGNATURE)
    {
        throw std::invalid_argument("Invalid NT Header");
    }
}

bool PEImage::getSectionPointer(const std::string& sectionName, uint8_t* &sectionPtr, size_t &sectionSize)
{
    sectionPtr = nullptr;
    sectionSize = 0;

    auto sectionHeader = getSectionHeader(sectionName);
    sectionPtr = m_imagePtr + sectionHeader->PointerToRawData;
    sectionSize = sectionHeader->Misc.VirtualSize;
    return true;
}


PIMAGE_SECTION_HEADER PEImage::getSectionHeader(const std::string &sectionName)
{
    auto numSections = m_ntHeader->FileHeader.NumberOfSections;

    auto sectionHeader = IMAGE_FIRST_SECTION(m_ntHeader);
    for (int i = 0; i < numSections; ++i, ++sectionHeader)
    {
        //printf("%d: %s\n", i, PEGetString(peBase, reinterpret_cast<char*>(sectionHeader->Name)));
        if (!strcmp(PEGetString(reinterpret_cast<char*>(sectionHeader->Name)), sectionName.c_str()))
        {
            return sectionHeader;
        }
    }
    return nullptr;
}

const char* const PEImage::PEGetString(const char* const string)
{
    if (string[0] != '/')
    {
        return string;
    }
    int idx;
    if (1 != sscanf(string + 1, "%d", &idx))
    {
        char buff[121];
        sprintf(buff, "Error reading string table entry index! %p", string);
        printf("%s\n", buff);
        gets(buff);
        throw std::runtime_error(buff);
    }
    return PEGetStringTableEntry(idx);
}

const char * const PEImage::PEGetStringTableEntry(size_t stringTableOffset)
{
    auto numSymbols = m_ntHeader->FileHeader.NumberOfSymbols;
    auto symbolTable = reinterpret_cast<PIMAGE_SYMBOL>(m_imagePtr + m_ntHeader->FileHeader.PointerToSymbolTable);

    auto stringTableStart = reinterpret_cast<char*>(symbolTable + numSymbols);

    auto ptr = stringTableStart + stringTableOffset;
    return ptr;
}


