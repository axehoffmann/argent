#pragma once

#include <string>
#include <stdexcept>

namespace ag 
{
    struct Resource
    {

        Resource(std::string path);

        std::string filePath;

        /**
        * Returns whether the asset data is loaded into memory and ready for use
        */
        virtual bool IsReady()=0;
        /**
        * Loads the asset to memory
        */
        virtual void Load()=0;
        virtual void Unload()=0;

    };
}