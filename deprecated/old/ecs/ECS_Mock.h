#pragma once

#include "ecs/ECSTypes.h"

namespace mock
{
    struct CmptA
    {
        int value;
        CmptA(int val)
        {
            value = val;
        }
    };

    struct CmptB
    {
        EntityID partner;
        CmptB(EntityID p)
        {
            partner = p;
        }
    };
}