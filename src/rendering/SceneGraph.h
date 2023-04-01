#pragma once

#include <unordered_map>
#include "../core/Transform.h"
#include "../ecs/ECSTypes.h"

namespace ag
{
    /**
     * Static Renderables do not need to be extrapolated, or have their transform updated each tick
    */
    struct StaticRenderable
    {
        EntityID entityID;

        uint32_t meshID;
        uint32_t materialID;

        ag::Transform transform;
    };
    
    /**
     * Stores a minimal representation of the scene to render
    */
    struct SceneGraph
    {
        std::unordered_map<EntityID, StaticRenderable> statics;
        // std::unordered_map<EntityID, DynamicRenderable> dynamics;

    };
}