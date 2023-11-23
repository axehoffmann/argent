#include "tests/TestFramework.h"
#include "ecs/archetypes/archetype.h"
#include "ecs/components/component.h"

namespace arch_test
{
	$UTest(archetype);

	$Case(basic, archetype)
	{
		std::cout << ag::componentID<std::remove_cv_t<ag::Transform>> << std::endl;
		auto set = ag::make_component_set<MAX_COMPONENTS, ag::Transform>();
		std::cout << "pluh" << set.at(0) << std::endl;
		ag::archetype arch(set);
		
		ag_expect(arch.count() == 0, "");

		glm::vec3 expectedPos{1,0,1};
		arch.instantiateImmediate(ag::Transform(expectedPos));

		ag_expect(arch.count() == 1, "");

	}
}