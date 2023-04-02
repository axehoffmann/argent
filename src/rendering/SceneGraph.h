#pragma once

#include <vector>
#include <unordered_map>
#include "../core/Transform.h"
#include "../ecs/ECSTypes.h"

namespace ag
{
    /**
     * Static Renderables do not need to be extrapolated, or have their transform updated each tick
    */
    struct StaticRenderInstance
    {
        EntityID entityID;

        uint32_t meshID;
        uint32_t materialID;

        ag::Transform transform;

        StaticRenderInstance(EntityID id, uint32_t mesh, uint32_t mat, ag::Transform tr) : entityID(id), meshID(mesh), materialID(mat), transform(tr) {}
    };
    
    /**
     * Stores a minimal representation of the scene to render
    */
    struct SceneGraph
    {
        std::vector<StaticRenderInstance> statics;
        // std::unordered_map<EntityID, DynamicRenderInstance> dynamics;

    };
}