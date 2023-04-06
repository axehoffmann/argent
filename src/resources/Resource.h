#pragma once

#include <string>
#include <stdexcept>

namespace ag 
{
    struct Resource
    {
        Resource(std::string path);

        std::string filePath;

        virtual bool IsReady()=0;
        virtual void Load()=0;
        virtual void Unload()=0;

    };
}