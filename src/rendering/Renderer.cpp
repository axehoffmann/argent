#include "Renderer.h"

ag::Renderer::Renderer(ag::RenderEngine* engine, ag::SceneBuilder* scene)
{
    renderEngine = engine;
    sceneBuilder = scene;
}

bool ag::Renderer::Initialise()
{
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
