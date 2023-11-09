#include "../ecs/ECSTypes.h"
#include "../core/Transform.h"

#include "../resources/AssetManager.h"
#include "../resources/Material.h"
#include "../resources/Mesh.h"

namespace ag
{
    /**
     * Static Renderables do not need to be extrapolated, or have their transform updated each tick
    */
    struct StaticRenderable
    {
        uint32_t meshID;
        uint32_t materialID;

        static inline auto serialiser = ag::Component::Serialiser<StaticRenderable>("static_renderable");

        static nlohmann::json ToJSON(StaticRenderable t);
        static StaticRenderable FromJSON(nlohmann::json& ob);
    };
}