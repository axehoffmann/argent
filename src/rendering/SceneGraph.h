#pragma once

#include <vector>
#include <unordered_map>
#include "lighting/PointLight.h"    
#include "../core/Transform.h"
#include "../ecs/ECSTypes.h"

#include "lib/map.h"

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

    struct DynamicRenderInstance
    {
        EntityID entityID;

        uint32_t meshID;
        uint32_t materialID;

        ag::Transform transform;

        /// TODO: Determine whether we should do extrapolation or interpolation
        glm::vec3 velocity;

        DynamicRenderInstance(EntityID id, uint32_t mesh, uint32_t mat, ag::Transform tr, glm::vec3 vel) : entityID(id), meshID(mesh), materialID(mat), transform(tr), velocity(vel) {}
    };
    
    /**
     * Stores a minimal representation of the scene to render
    */
    struct SceneGraph
    {
        std::vector<StaticRenderInstance> statics;
        map<EntityID, DynamicRenderInstance> dynamics;

        std::vector<ag::PointLight> pointLights;

    };
}