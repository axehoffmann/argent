#include "SceneBuilder.h"
#include "debug/log/Log.h"

ag::SceneBuilder::SceneBuilder(std::shared_ptr<ag::World> w) : 
    staticQuery(w.get()),
    lightQuery(w.get()),
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
        EntityID id = entity.Info();

        graph->statics.emplace_back(id, 
            entity.Get<StaticRenderable>().meshID, 
            entity.Get<StaticRenderable>().materialID,
            entity.Get<Transform>());
    }

    graph->pointLights.clear();
    graph->pointLights.reserve(lightQuery.Size());
    for (auto entity : lightQuery)
    {
        graph->pointLights.push_back(PointLight{ entity.Get<Transform>().GetPosition(), entity.Get<PointLightComp>().colour, 1 });
    }

    graphReady = lockedGraph;
    graphUnderConstruction = -1;
}

const ag::SceneGraph& ag::SceneBuilder::StartGraphRead()
{
    for (int i = 0; i < GRAPH_BUFFER_SIZE; i++)
    {
        if (i == graphReady && i != graphUnderConstruction && i != graphReadByRenderer)
        {
            graphReadByRenderer = i;

            return graphs[i];
        }
    }

    Log::Error("Couldn't find a SceneGraph to read this frame");
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
