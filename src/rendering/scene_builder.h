#pragma once

#include "mesh_handler.h"
#include "render_object.h"
#include "scene_graph.h"
#include "transform.h"

#include "ecs/ECS.h"
#include "ecs/Query.h"

#include <memory>
#include "lib/vector.h"
#include "lib/sync.h"

namespace ag
{
    constexpr u16 graph_buffering = 3;

    /// TODO: gather buffer of deleted and created objects, update scene graph with new objects that way, rather than clearing and rebuilding it each frame?

    /**
     * Runs every fixed gametick, building a structure of all the objects to render.
    */
    class scene_builder
    {
    public:
        scene_builder(std::shared_ptr<ag::World> w, mesh_handler& m);

        /**
         * Generates a new graph at the end of each gametick
        */
        void Update();

        /**
         * Called by the Renderer to get the most recent completed scene graph
        */
        scene_graph& StartGraphRead();

        /**
         * Called by the Renderer to stop locking the graph
        */
        void EndGraphRead();

    private:

        ag::Query<transform, render_object> staticQuery;
        ag::Query<transform, render_object> lightQuery;

        // Chooses the location of the next graph to construct and updates atomic indices
        int ChooseGraph();
        // Graph currently owned by the SceneBuilder
        atomic<int> graphUnderConstruction;
        // Graph currently owned by the Renderer
        atomic<int> graphReadByRenderer;
        // Next graph for the Renderer to read
        atomic<int> graphReady;
        vector<scene_graph> graphs;

        mesh_handler& mesh_info;

        std::shared_ptr<ag::World> world;
    };
}