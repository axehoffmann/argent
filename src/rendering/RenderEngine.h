#pragma once

#include "SceneBuilder.h"
#include "../resources/Mesh.h"

namespace ag
{
    class RenderEngine
    {
    public:
        RenderEngine();
        
        virtual void Render(ag::SceneGraph* scene) = 0;

        virtual void LoadMesh(uint32_t meshID) = 0;
        virtual void UnloadMesh(uint32_t meshID) = 0;

        virtual void LoadMaterial(uint32_t materialID) = 0;
        virtual void UnloadMaterial(uint32_t materialID) = 0;


    private:
    };
}