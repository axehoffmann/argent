#include "scene_builder.h"
#include "debug/log/Log.h"

#include "transform.h"

ag::scene_builder::scene_builder(std::shared_ptr<ag::World> w, mesh_handler& m) :
    mesh_info(m),
    staticQuery(w.get()),
    lightQuery(w.get()),
    world(w),
    graphUnderConstruction(-1),
    graphReadByRenderer(-1),
    graphReady(-1),
    graphs(3)
{}

glm::vec4 normalizePlane(glm::vec4 plane)
{
    return plane / glm::length(glm::vec3(plane));
}

void ag::scene_builder::Update()
{
    // Choose which graph to use
    int lockedGraph = ChooseGraph();

    scene_graph& graph = graphs[lockedGraph];

    /// TODO: bad approach to building scene graph, do incremental updates
    graph.scene.clear();
    graph.meshCounts.clear();

    for (auto entity : staticQuery)
    {
        render_object& r = entity.Get<render_object>();
        transform& tr = entity.Get<transform>();    

        glm::vec4 baseSphere = mesh_info.baseBoundingSpheres[r.meshID];
        glm::vec4 boundingSphere = glm::vec4(
            tr.pos + glm::vec3(baseSphere), 
            baseSphere.w * std::max(tr.scale.x, std::max(tr.scale.y, tr.scale.z))
        );

        graph.scene.push_back({ model_matrix(tr), boundingSphere, r.materialID, r.meshID });

        if (r.meshID >= graph.meshCounts.size())
            graph.meshCounts.resize(u64(r.meshID) + 1);
        graph.meshCounts[r.meshID]++;
    }

    /// TODO: parameterise camera data
    auto proj = projection_matrix(glm::radians(90.0f), 1280.0f / 720.0f, 200.0f, 0.01f);
    proj[1][1] *= -1;
    proj = glm::transpose(proj);
    glm::vec4 fX = normalizePlane(proj[3] + proj[0]);
    glm::vec4 fY = normalizePlane(proj[3] + proj[1]);

    graph.info.totalObjects = u32(graph.scene.size());
    graph.info.view = view_matrix({0, 0, 2}, 0, 0);
    graph.info.frustum[0] = fX.x;
    graph.info.frustum[1] = fX.z;
    graph.info.frustum[2] = fY.y;
    graph.info.frustum[3] = fY.z;
    graph.info.proj_00 = proj[0][0];
    graph.info.proj_11 = proj[1][1];
    graph.info.zNear = 0.01f;
    graph.info.zFar = 200.0f;
    graph.info.hzbWidth = 2560;
    graph.info.hzbHeight = 1440;

    static bool once = false;
    if (not once)
    {
        once = true;
        ag::Log::Trace(ag::sfmt("{} {} {} {}", fX.x, fX.z, fY.y, fY.z));
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

scene_graph& ag::scene_builder::StartGraphRead()
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
