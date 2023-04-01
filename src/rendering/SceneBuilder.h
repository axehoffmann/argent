#pragma once

#include "SceneGraph.h"
#include <atomic>

#define GRAPH_BUFFER_SIZE 3

namespace ag
{
    /**
     * Runs every fixed gametick, building a structure of all the objects to render.
    */
    class SceneBuilder
    {
    public:
        SceneBuilder();

        /**
         * Generates a new graph at the end of each gametick
        */
        void Update();

        /**
         * Called by the Renderer to get the most recent completed scene graph
        */
        ag::SceneGraph* GetRecentGraph();        

    private:

        // Graph currently owned by the SceneBuilder
        std::atomic<int> graphUnderConstruction;
        // Graph currently owned by the Renderer
        std::atomic<int> graphReadByRenderer;
        // Next graph for the Renderer to read
        std::atomic<int> graphReady;
        std::array<ag::SceneGraph, GRAPH_BUFFER_SIZE> graphs;


        // ecs world reference
    };
}