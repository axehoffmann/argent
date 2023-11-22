#include "tests/TestFramework.h"
#include "ecs/archetypes/archetype.h"

namespace arch_test
{
	$UTest(archetype);

	$Case(basic, archetype)
	{
		ag::archetype arch(ag::make_component_set<MAX_COMPONENTS, ag::Transform>());
		
		ag_expect(arch.count() == 0, "");

		glm::vec3 expectedPos{1,0,1};
		arch.instantiateImmediate(ag::Transform(expectedPos));

		ag_expect(arch.count() == 1, "");

	}
}