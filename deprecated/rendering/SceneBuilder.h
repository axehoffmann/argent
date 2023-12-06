#pragma once

#include "SceneGraph.h"
#include "../ecs/ECS.h"
#include "../ecs/Query.h"
#include "StaticRenderable.h"

#include <memory>
#include <atomic>
#include <array>

#define GRAPH_BUFFER_SIZE 3

namespace ag
{

    /// TODO: gather buffer of deleted and created objects, update scene graph with new objects that way, rather than clearing and rebuilding it each frame?

    /**
     * Runs every fixed gametick, building a structure of all the objects to render.
    */
    class SceneBuilder
    {
    public:
        SceneBuilder(std::shared_ptr<ag::World> w);

        /**
         * Generates a new graph at the end of each gametick
        */
        void Update();

        /**
         * Called by the Renderer to get the most recent completed scene graph
        */
        const ag::SceneGraph& StartGraphRead();

        /**
         * Called by the Renderer to stop locking the graph
        */
        void EndGraphRead();        

    private:
        
        ag::Query<Transform, StaticRenderable> staticQuery;
        ag::Query<Transform, PointLightComp> lightQuery;

        // Chooses the location of the next graph to construct and updates atomic indices
        int ChooseGraph();
        // Graph currently owned by the SceneBuilder
        std::atomic<int> graphUnderConstruction;
        // Graph currently owned by the Renderer
        std::atomic<int> graphReadByRenderer;
        // Next graph for the Renderer to read
        std::atomic<int> graphReady;
        std::array<ag::SceneGraph, GRAPH_BUFFER_SIZE> graphs;

        std::shared_ptr<ag::World> world;
    };
}