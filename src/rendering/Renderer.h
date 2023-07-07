#pragma once

#include "RenderEngine.h"

#include <memory>

namespace ag
{
    /**
     * Manages synchronisation of the SceneBuilder across threads, 
     *     and the execution of the RenderEngine (library-specific implementation)
    */
    class Renderer
    {
    public:
        Renderer(std::shared_ptr<ag::RenderEngine> engine, std::shared_ptr<ag::SceneBuilder> scene);
        
        bool Initialise();

        void Render();
    
    private:
        std::shared_ptr<ag::RenderEngine> renderEngine;
        std::shared_ptr<ag::SceneBuilder> sceneBuilder;
    };
}
