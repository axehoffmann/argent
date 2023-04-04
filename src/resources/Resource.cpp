#include "Resource.h"

std::atomic<uint32_t> nextID(0);

ag::Resource::Resource(std::string path)
{
    filePath = path;
    ID = ++nextID;
}