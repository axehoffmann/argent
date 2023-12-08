#include "scene_builder.h"
#include "debug/log/Log.h"

ag::scene_builder::scene_builder(std::shared_ptr<ag::World> w) :
    staticQuery(w.get()),
    lightQuery(w.get()),
    world(w),
    graphUnderConstruction(-1),
    graphReadByRenderer(-1),
    graphReady(-1),
    graphs(3)
{}

void ag::scene_builder::Update()
{
    // Choose which graph to use
    int lockedGraph = ChooseGraph();

    scene_graph* graph = &graphs[lockedGraph];

    /// TODO: bad approach to building scene graph
    for (auto v : graph->scene)
    {
        v.clear();
    }

    for (auto entity : staticQuery)
    {
        render_object& r = entity.Get<render_object>();

        if (graph->scene.size() <= r.meshID)
            graph->scene.resize(u64{r.meshID} + 1);

        graph->scene.at(r.meshID).push_back({ model_matrix(entity.Get<transform>()) });
    }

    /*
    graph->pointLights.clear();
    graph->pointLights.reserve(lightQuery.Size());
    for (auto entity : lightQuery)
    {
        graph->pointLights.push_back(PointLight{ entity.Get<Transform>().GetPosition(), entity.Get<PointLightComp>().colour, 1 });
    }
    */

    graphReady = lockedGraph;
    graphUnderConstruction = -1;
    
}

const scene_graph& ag::scene_builder::StartGraphRead()
{
    for (int i = 0; i < graphs.size(); i++)
    {
        if (i == graphReady && i != graphUnderConstruction && i != graphReadByRenderer)
        {
            graphReadByRenderer = i;

            return graphs[i];
        }
    }

    Log::Error("Couldn't find a SceneGraph to read this frame");
}

void ag::scene_builder::EndGraphRead()
{
    graphReadByRenderer = -1;
}

int ag::scene_builder::ChooseGraph()
{
    for (int i = 0; i < graphs.size(); i++)
    {
        if (i != graphReadByRenderer && i != graphReady)
        {
            graphUnderConstruction = i;
            return i;
        }
    }
    return -1;
}
