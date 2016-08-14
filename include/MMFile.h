#pragma once

#include <Windows.h>
#include <string>
#include <stdexcept>

enum class MMOpenMode {
    read,
    readWrite
};

class MMFileException : public std::runtime_error
{
public:
    template <typename T>
    MMFileException(T&& msg) : runtime_error(std::forward<T>(msg)) {}
};
#define MMFenforce(cond, msg) if(!(cond)) { throw MMFileException((msg)); }

class MMFile
{
    HANDLE fileHandle;
    HANDLE mapHandle;
    void* ptr;
    size_t fileSize;
public:
    MMFile(const std::string &path, MMOpenMode mode, size_t _sizeToMap = 0)
    {
        auto desiredAccess = mode == MMOpenMode::read ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE;
        auto shareMode = mode == MMOpenMode::read ? FILE_SHARE_READ : FILE_SHARE_READ | FILE_SHARE_WRITE;
        auto createDisposition = mode == MMOpenMode::read ? OPEN_EXISTING : OPEN_ALWAYS;
        fileHandle = CreateFile(path.c_str(), desiredAccess, shareMode, NULL, createDisposition, 0, NULL);
        MMFenforce(fileHandle != INVALID_HANDLE_VALUE, "Could not open desired file: " + path);
        LARGE_INTEGER size;
        size.QuadPart = _sizeToMap;
        if (size.QuadPart == 0) {
            size.LowPart = GetFileSize(fileHandle, (LPDWORD)&size.HighPart);
        }
        fileSize = (size_t)size.QuadPart;
        MMFenforce(fileSize, std::string("File is of 0 size, cant map that! ") + path);


        auto protectMode = mode == MMOpenMode::read ? PAGE_READONLY : PAGE_READWRITE;
        mapHandle = CreateFileMapping(fileHandle, NULL, protectMode, size.HighPart, size.LowPart, NULL);
        MMFenforce(mapHandle != NULL, "Could not create file mapping for file " + path);
        auto mapAccess = mode == MMOpenMode::read ? FILE_MAP_READ : FILE_MAP_WRITE;
        ptr = MapViewOfFileEx(mapHandle, mapAccess, 0, 0, fileSize, NULL);
        MMFenforce(ptr != NULL, "Could not map view of file");
    }
    virtual ~MMFile()
    {
        dispose();
    }
    void dispose()
    {
        fileSize = 0;
        if (ptr) {
            FlushViewOfFile(ptr, fileSize);
            UnmapViewOfFile(ptr); ptr = nullptr;
        }
        if (mapHandle != NULL) {
            CloseHandle(mapHandle); mapHandle = INVALID_HANDLE_VALUE;
        }
        if (fileHandle != INVALID_HANDLE_VALUE) {
            CloseHandle(fileHandle); fileHandle = INVALID_HANDLE_VALUE;
        }
    }

    void* getPtr() {
        return ptr;
    }
    size_t getSize() {
        return fileSize;
    }

    template <typename A>
    void get(A& out, size_t offset) {
        out = A(((char*)ptr) + offset);
    }
};

