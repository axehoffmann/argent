#include "../TestFramework.h"
#include "core/spatial/Grid.h"

#include <glm/glm.hpp>

void data_access()
{
    ag::Grid<int, int> gr(5, 5, 10.0f);
    
    std::vector<int> q = gr.Query(glm::vec3(25.0f, 25.0f, 0.0f));
    ag_expect(q.size() == 0, "Expected 0 query results on an empty grid");

    gr.Insert(0, 0, glm::vec3(9.0f, 9.0f, 9.0f));
    q = gr.Query(glm::vec3(11.0f, 11.0f, 0.0f));
    ag_expect(q.size() == 1, "Expected 1 query result on an occupied grid");

    q = gr.Query(glm::vec3(40.0f, 40.0f, 0.0f));
    ag_expect(q.size() == 0, "Expected 0 query results on an occupied grid");

    gr.Insert(1, 1, glm::vec3(15.0f, 15.0f, 0.0f));
    q = gr.Query(glm::vec3(11.0f, 11.0f, 0.0f));
    ag_expect(q.size() == 2, "Expected 2 query results");

    q = gr.Query(glm::vec3(21.0f, 21.0f, 0.0f));
    ag_expect(q.size() == 1, "Expected 1 query result");
}

void moving()
{
    ag::Grid<int, int> gr(5, 5, 10.0f);

    std::vector<int> q = gr.Query(glm::vec3(25.0f, 25.0f, 0.0f));

    gr.Insert(0, 0, glm::vec3(10.0f, 10.0f, 0.0f));
    q = gr.Query(glm::vec3(11.0f, 11.0f, 0.0f));
    ag_expect(q.size() == 1, "Expected 1 query result on an occupied grid");

    gr.Move(0, glm::vec3(40.0f, 40.0f, 0.0f));
    q = gr.Query(glm::vec3(11.0f, 11.0f, 0.0f));
    ag_expect(q.size() == 0, "Expected 0 query results after object moved");
}

void many_objects()
{
    ag::Grid<int, int> gr(5, 5, 10.0f);

    int id = 0;
    for (size_t x = 15; x < 40; x += 10)
    {
        for (size_t y = 15; y < 40; y += 10)
        {
            for (size_t i = 0; i < 2; i++)
            {
                gr.Insert(id, id, glm::vec3(x, y, 0.0f));
                id++;
            }
        }
    }

    std::vector<int> expected = { 2,  4,  6,  4,  2,
                                  4,  8, 12,  8,  4,
                                  6, 12, 18, 12,  6,
                                  4,  8, 12,  8,  4,
                                  2,  4,  6,  4,  2 };

    int index = 0;
    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < 5; j++)
        {
            std::vector<int> q = gr.Query(glm::vec3(5.0f + 10.0f * i, 5.0f + 10.0f * j, 0.0f));
            ag_expect(q.size() == expected.at(index), "Expected {} query results at ({}, {}), instead found {}", expected.at(index), 5 + 10 * i, 5 + 10 * j, q.size());
            index++;
        }
    }
}

int main()
{
    Test::Name("Grids");

    Test::Case("Data Access", data_access);
    Test::Case("Moving Objects", moving);
    Test::Case("Many Objects", many_objects);

    Test::Run();
}