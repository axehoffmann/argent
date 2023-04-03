#pragma once

#include "RenderEngine.h"

namespace ag
{
    /**
     * Manages synchronisation of the SceneBuilder across threads, and the execution of the RenderEngine (library-specific implementation)
    */
    class Renderer
    {
    public:
        Renderer(ag::RenderEngine* engine, ag::SceneBuilder* scene);
        
        bool Initialise();

        void Render();
    
    private:
        ag::RenderEngine* renderEngine;
        ag::SceneBuilder* sceneBuilder;
    };
}
