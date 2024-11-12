#include "include.h"
#pragma comment(lib, "physmeme-lib.lib")

namespace physmeme
{
    NTSTATUS __cdecl map_driver(std::vector<std::uint8_t>& raw_driver);
    NTSTATUS __cdecl map_driver(std::uint8_t* image, std::size_t size);
}