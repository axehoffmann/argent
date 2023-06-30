#include "StaticRenderable.h"

ag::Component::Serialiser<ag::StaticRenderable> ag::StaticRenderable::serialiser("static_renderable");

nlohmann::json ag::StaticRenderable::ToJSON(StaticRenderable t)
{
    nlohmann::json ob;
    ob["type"] = "static_renderable";
    /// TODO: this doesn't re-serialise the paths of the resources

    return ob;
}

ag::StaticRenderable ag::StaticRenderable::FromJSON(nlohmann::json & ob)
{
    StaticRenderable r;
    r.meshID = ag::AssetManager::Load<ag::Mesh>(ob["mesh"]);
    r.materialID = ag::AssetManager::Load<ag::Material>(ob["material"]);

    return r;
}