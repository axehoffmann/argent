#pragma once

#include "SceneGraph.h"
#include "../ecs/ECS.h"
#include "StaticRenderable.h"
#include <atomic>
#include <array>

#include <iostream>

#define GRAPH_BUFFER_SIZE 3

namespace ag
{
    /**
     * Runs every fixed gametick, building a structure of all the objects to render.
    */
    class SceneBuilder
    {
    public:
        SceneBuilder(ag::World* w);

        /**
         * Generates a new graph at the end of each gametick
        */
        void Update();

        /**
         * Called by the Renderer to get the most recent completed scene graph
        */
        ag::SceneGraph* StartGraphRead();
        /**
         * Called by the Renderer to stop locking the graph
        */
        void EndGraphRead();        

    private:
        // Chooses the location of the next graph to construct and updates atomic indices
        int ChooseGraph();
        // Graph currently owned by the SceneBuilder
        std::atomic<int> graphUnderConstruction;
        // Graph currently owned by the Renderer
        std::atomic<int> graphReadByRenderer;
        // Next graph for the Renderer to read
        std::atomic<int> graphReady;
        std::array<ag::SceneGraph, GRAPH_BUFFER_SIZE> graphs;

        ag::World* world;
    };
}