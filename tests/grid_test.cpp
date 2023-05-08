#include "TestFramework.h"
#include "../src/core/Grid.h"

#include <glm/glm.hpp>

void data_access()
{
    ag::Grid<int, int> gr(5, 5, 10.0f);
    
    std::vector<int> q = gr.Query(glm::vec3(25.0f, 25.0f, 0.0f));
    ag_expect(q.size() == 0, "Expected 0 query results on an empty grid");

    gr.Insert(0, 0, glm::vec3(10.0f, 10.0f, 0.0f));
    q = gr.Query(glm::vec3(11.0f, 11.0f, 0.0f));
    ag_expect(q.size() == 1, "Expected 1 query result on an occupied grid");

    q = gr.Query(glm::vec3(40.0f, 40.0f, 0.0f));
    ag_expect(q.size() == 0, "Expected 0 query results on an occupied grid");

    gr.Insert(1, 1, glm::vec3(15.0f, 15.0f, 0.0f));
    q = gr.Query(glm::vec3(11.0f, 11.0f, 0.0f));
    ag_expect(q.size()) == 2, "Expected 2 query results");

    q = gr.Query(glm::vec3(21.0f, 21.0f, 0.0f));
    ag_expect(q.size()) == 1, "Expected 1 query result");
}

void moving()
{
    ag::Grid<int, int> gr(5, 5, 10.0f);

    std::vector<int> q = gr.Query(glm::vec3(25.0f, 25.0f, 0.0f));

    gr.Insert(0, 0, glm::vec3(10.0f, 10.0f, 0.0f));
    q = gr.Query(glm::vec3(11.0f, 11.0f, 0.0f));
    ag_expect(q.size() == 1, "Expected 1 query result on an occupied grid");

    gr.Move(0, glm::vec3(0.0f, 0.0f, 0.0f));
    q = gr.Query(glm::vec3(11.0f, 11.0f, 0.0f));
    ag_expect(q.size() == 0, "Expected 0 query results after object moved");
}

int main()
{
    Test::Name("Transforms");

    Test::Case("Data Access", data_access);
    Test::Case("Moving Onjects", moving);

    Test::Run();
}