#pragma once

#include "SceneBuilder.h"
#include "../resources/Mesh.h"

namespace ag
{
    class RenderEngine
    {
    public:

        virtual void Render(const ag::SceneGraph& scene) = 0;

    protected:

        // virtual void InitMesh(uint32_t meshID) = 0;
        // virtual void InitMaterial(uint32_t materialID) = 0;

    };
}