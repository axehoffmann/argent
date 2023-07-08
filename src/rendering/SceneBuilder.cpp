#include "SceneBuilder.h"

ag::SceneBuilder::SceneBuilder(std::shared_ptr<ag::World> w) : 
    staticQuery(w.get()),
    world(w),
    graphUnderConstruction(-1),
    graphReadByRenderer(-1),
    graphReady(-1) {}

void ag::SceneBuilder::Update()
{
    // Choose which graph to use
    int lockedGraph = ChooseGraph();

    ag::SceneGraph* graph = &graphs[lockedGraph];
    
    /// TODO: static renderable graph should not be rebuilt every update. It should only be edited based on spawned and destroyed entities this update.
    graph->statics.clear();

    graph->statics.reserve(staticQuery.Size());

    for (auto entity : staticQuery)
    {
        EntityID id = entity.Info().ID;

        graph->statics.emplace_back(id, 
            entity.Get<StaticRenderable>().materialID, 
            entity.Get<StaticRenderable>().meshID, 
            entity.Get<Transform>());
    }

    graphReady = lockedGraph;
    graphUnderConstruction = -1;
}

ag::SceneGraph* ag::SceneBuilder::StartGraphRead()
{
    for (int i = 0; i < GRAPH_BUFFER_SIZE; i++)
    {
        if (i == graphReady && i != graphUnderConstruction && i != graphReadByRenderer)
        {
            graphReadByRenderer = i;

            return &graphs[i];
        }
    }

    return nullptr;
}

void ag::SceneBuilder::EndGraphRead()
{
    graphReadByRenderer = -1;
}

int ag::SceneBuilder::ChooseGraph()
{
    for (int i = 0; i < GRAPH_BUFFER_SIZE; i++)
    {
        if (i != graphReadByRenderer && i != graphReady)
        {
            graphUnderConstruction = i;
            return i;
        }
    }
    return -1;
}
