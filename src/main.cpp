#include <windows.h>
#include <stdio.h>
#include <string>
#include "MMFile.h"
#include <memory>
#include <stdexcept>

#include "dwarf.h"
#include "image.h"
#include "peimage.h"
#include "dwarf_reader.h"


#if((__GNUC__ <= 4) && (__GNUC_MINOR__ <= 8))
// ugh, what standard library does not have this? >_<
namespace std{
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

#endif

int main(int argc, char* argv[])
{
    try
    {
        std::string src = argv[0];
        if (argc == 2)
        {
            src = argv[1];
        }

        auto file = std::make_unique<MMFile>(src.c_str(), MMOpenMode::read);
        auto peBase = file->getPtr();

        std::shared_ptr<Image> image = std::make_shared<PEImage>();
        image->loadImage(peBase, 0x0);

        uint8_t* debug_info;
        size_t debug_info_size;
        image->getSectionPointer(".debug_info", debug_info, debug_info_size);
        printf("Using source: %s\n", src.c_str());
        printf("Section %p (%X)\n", debug_info, debug_info_size);

        auto dwarfReader = std::make_shared<DwarfReader>(image);

        auto nothing = [](const DwarfTag& a, ptrdiff_t level){ 
            //printf("ASDASDASDASD\n\n\n");
            return true;
        };
        for (size_t index = 0; index < dwarfReader->getCompilationUnitCount(); ++index)
        {
            auto compUnit = dwarfReader->getCompilationUnit(index);
            auto compUnitTag = compUnit.getFirstTag();
            compUnitTag.forAllTagsAndChildren(nothing);
            system("pause");
        }

        system("pause");
    }
    catch (const std::exception& e)
    {
        MessageBoxA(NULL, e.what(), "Exception caught", MB_ICONEXCLAMATION);
    }
}

