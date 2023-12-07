#pragma once

#include <stdexcept>

#include "lib/string.h"

namespace ag 
{
    struct Resource
    {

        Resource(const string& path);

        string filePath;

        /**
        * Returns whether the asset data is loaded into memory and ready for use
        */
        virtual bool IsReady() const =0;
        /**
        * Loads the asset to memory
        */
        virtual void Load()=0;
        virtual void Unload()=0;

    };
}