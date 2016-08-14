#pragma once

#include <cstddef>
#include <string>

class Image
{
public:
    Image() = default;
    Image(const Image&) = delete;
    void operator=(const Image&) = delete;

    virtual ~Image(){}
    virtual void dispose() = 0;

    virtual void loadImage(void* imageData, size_t virtualAddress) = 0;
    virtual bool getSectionPointer(const std::string& sectionName, uint8_t* &sectionPtr, size_t &sectionSize) = 0;
};

