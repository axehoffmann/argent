#include "render_object.h"

#include "resources/AssetManager.h"
#include "resources/Material.h"
#include "resources/Mesh.h"

nlohmann::json ag::render_object::ToJSON(const ag::render_object& t)
{
    nlohmann::json ob;
    ob["type"] = "static_renderable";
    /// TODO: this doesn't re-serialise the paths of the resources

    return ob;
}

ag::render_object ag::render_object::FromJSON(nlohmann::json& ob)
{
    return render_object {
        ob["mesh"],
        ob["material"]
    };
}