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

        static nlohmann::json ToJSON(StaticRenderable t)
        {
            nlohmann::json ob;
            ob["type"] = "static_renderable";
            /// TODO: this doesn't re-serialise the paths of the resources
        }
        static StaticRenderable FromJSON(nlohmann::json& ob)
        {
            StaticRenderable r;
            r.meshID = ag::AssetManager::Load<ag::Mesh>(ob["mesh"]);
            r.materialID = ag::AssetManager::Load<ag::Material>(ob["material"]);
        }
    };
}