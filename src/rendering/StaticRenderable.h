#include "../ecs/ECSTypes.h"
#include "../core/Transform.h"

namespace ag
{
    /**
     * Static Renderables do not need to be extrapolated, or have their transform updated each tick
    */
    struct StaticRenderable
    {
        uint32_t meshID;
        uint32_t materialID;
    };
}