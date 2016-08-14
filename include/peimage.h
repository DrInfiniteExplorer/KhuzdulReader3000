#pragma once

#include "image.h"
#include <windows.h>

class PEImage : public Image
{
public:
    PEImage(); 
    
    virtual void dispose() override;

    virtual void loadImage(void* imageData, size_t virtualAddress) override;

    virtual bool getSectionPointer(const std::string& sectionName, uint8_t* &sectionPtr, size_t &sectionSize) override;

    PIMAGE_SECTION_HEADER getSectionHeader(const std::string &sectionName);

    const char * const PEGetStringTableEntry(size_t stringTableOffset);

    const char* const PEGetString(const char* const string);



private:
    uint8_t* m_imagePtr;
    size_t m_virtualBase;
    PIMAGE_DOS_HEADER m_dosHeader;
    PIMAGE_NT_HEADERS m_ntHeader;
};


