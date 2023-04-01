#pragma once

#include "SceneBuilder.h"
#include "../resources/Mesh.h"

namespace ag
{
    class RenderEngine
    {
    public:
        RenderEngine(ag::SceneBuilder* scene);
        
        virtual void Render() = 0;

        virtual void LoadMesh(uint32_t meshID) = 0;
        virtual void UnloadMesh(uint32_t meshID) = 0;

        virtual void LoadMaterial(uint32_t materialID) = 0;
        virtual void UnloadMaterial(uint32_t materialID) = 0;


    private:
        ag::SceneBuilder* renderScene;
    };
}