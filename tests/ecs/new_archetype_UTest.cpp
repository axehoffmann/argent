#include "tests/TestFramework.h"
#include "ecs/archetypes/archetype.h"
#include "ecs/components/component.h"

using namespace ag;

namespace arch_test
{
	$UTest(archetypes);

	$Case(basic, archetypes)
	{
		std::cout << ag::componentID<std::remove_cv_t<Transform>> << std::endl;
		auto set = ag::make_component_set<MAX_COMPONENTS, Transform>();

		archetype arch(set);
		
		ag_expect(arch.count() == 0, "");

		glm::vec3 expectedPos{1,0,1};
		arch.instantiateImmediate(Transform(expectedPos));

		archetype* ar = &arch;

		ag_expect(arch.count() == 1, "");

		// Construct an iterator to the archetype
		auto it = archetype::iterator<Transform>({&arch});
		
		auto [tf] = *it;

		ag_expect(tf.GetPosition() == expectedPos, "");
	}
}