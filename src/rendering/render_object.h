#pragma once

#include "ecs/ECSTypes.h"
#include "ecs/Component.h"


namespace ag
{
    struct render_object
    {
        u32 meshID;
        u32 materialID;

        static inline auto serialiser = ag::Component::Serialiser<render_object>("static_renderable");

        static nlohmann::json ToJSON(const render_object& t);
        static render_object FromJSON(nlohmann::json& ob);
    };
}