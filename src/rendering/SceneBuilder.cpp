#include "SceneBuilder.h"

ag::SceneBuilder::SceneBuilder(std::shared_ptr<ag::World> w)
{
    graphUnderConstruction = -1;
    graphReadByRenderer = -1;
    graphReady = -1;

    world = w;
}

void ag::SceneBuilder::Update()
{
    // Choose which graph to use
    int lockedGraph = ChooseGraph();

    ag::SceneGraph* graph = &graphs[lockedGraph];
    
    /// TODO: static renderable graph should not be rebuilt every update. It should only be edited based on spawned and destroyed entities this update.
    std::function<void(ag::QueryResult<ag::Transform>, ag::QueryResult<ag::StaticRenderable>)> lambda = [this, graph](ag::QueryResult<ag::Transform> tr, ag::QueryResult<ag::StaticRenderable> re)
    {
        graph->statics.clear();
        graph->statics.reserve(tr.Length());
        for (size_t i = 0; i < tr.Length(); i++)
        {
            EntityID id = tr.GetInfo(i).value().ID;
            graph->statics.emplace_back(id, re[i].materialID, re[i].meshID, tr[i]);
        }
    };

    world->Query(lambda);

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
