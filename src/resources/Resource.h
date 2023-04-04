#pragma once

#include <string>
#include <atomic>
#include <stdexcept>

namespace ag 
{
    struct Resource
    {
        Resource(std::string path);

        std::string filePath;
        uint32_t ID;

        virtual bool IsReady()=0;
        virtual void Load()=0;
        virtual void Unload()=0;

    private:
        static std::atomic<uint32_t> nextID;
    };
}