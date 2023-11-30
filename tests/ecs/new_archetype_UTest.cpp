#include "tests/test_framework.h"
#include "ecs/archetypes/archetype.h"
#include "ecs/components/component.h"

using namespace ag;

namespace arch_test
{
	$utest(archetypes);

	$tcase(basic, archetypes)
	{
		auto set = ag::make_component_set<MAX_COMPONENTS, Transform>();

		archetype arch(set);
		
		assert_equals(arch.count(), u64{0});

		glm::vec3 expectedPos{0,1,0};
		arch.instantiateImmediate(Transform(expectedPos));

		archetype* ar = &arch;

		assert_equals(arch.count(), u64{1});

		// Construct an iterator to the archetype
		auto it = archetype::iterator<Transform>({&arch});
		
		auto [tf] = *it;

		assert_equals(tf.GetPosition(), expectedPos);

		// This isn't the actual use-case in-engine, but it tests iteration and data format.
		for (u32 i = 1; i < 100; i++)
		{
			arch.instantiateImmediate(Transform({0, i + 1, 0}));
		}

		u32 i = 1;
		auto end = archetype::iterator<Transform>(0, 1);
		for (it = archetype::iterator<Transform>({ &arch }); it != end; it++)
		{
			auto [tf] = *it;
			expectedPos = { 0, i, 0 };
			assert_equals(tf.GetPosition(), expectedPos);
			i++;
		}
		assert_equals(i, u32{101});
	}
}