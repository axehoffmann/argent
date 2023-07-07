#include "Renderer.h"

ag::Renderer::Renderer(std::shared_ptr<ag::RenderEngine> engine, std::shared_ptr<ag::SceneBuilder> scene)
{
    renderEngine = engine;
    sceneBuilder = scene;
}

bool ag::Renderer::Initialise()
{
    renderEngine->Initialise();
    return true;
}

void ag::Renderer::Render()
{
    ag::SceneGraph* graph = sceneBuilder->StartGraphRead();

    /// TODO: CULL GRAPH
    /// TODO: Sort graph by model/material
    /// TODO: manage new resources to load/unload old resources
    renderEngine->Render(graph);
    
    // SwapBuffers()

    sceneBuilder->EndGraphRead();
}
