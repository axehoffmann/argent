#include "tests/test_framework.h"
#include "ecs/archetypes/archetype.h"
#include "ecs/components/component.h"

using namespace ag;

namespace arch_test
{
	const auto set = ag::make_component_set<MAX_COMPONENTS, transform>();

	$utest(archetypes);

	$tcase(basic, archetypes)
	{
		block_allocator alloc(1024);
		archetype arch(set, &alloc);
		
		assert_equals(arch.count(), u64{0});

		glm::vec3 expectedPos{0,1,0};
		arch.instantiateImmediate(transform(expectedPos));

		archetype* ar = &arch;

		assert_equals(arch.count(), u64{1});

		// Construct an iterator to the archetype
		auto it = archetype::iterator<transform>({&arch});
		
		auto [tf] = *it;

		assert_equals(tf.pos, expectedPos);

		// This isn't the actual use-case in-engine, but it tests iteration and data format.
		for (u32 i = 1; i < 100; i++)
		{
			arch.instantiateImmediate(transform({0, i + 1, 0}));
		}

		u32 i = 1;
		auto end = archetype::iterator<transform>(0, 1);
		for (it = archetype::iterator<transform>({ &arch }); it != end; it++)
		{
			auto [tf] = *it;
			expectedPos = { 0, i, 0 };
			assert_equals(tf.pos, expectedPos);
			i++;
		}
		assert_equals(i, u32{101});
	}

	$tcase(destruction, archetypes)
	{
		block_allocator alloc(1024);
		archetype arch(set, &alloc);

		arch.instantiateImmediate(transform({0, 0, 0}));

		assert_equals(arch.count(), u64{ 1 });

		arch.destroyAtIndex(0);
		arch.resolveBuffers();

		assert_equals(arch.count(), u64{ 0 });

		// Instantiate some entities, then destroy a subset
		for (u32 i = 0; i < 10; i++)
		{
			arch.instantiateImmediate(transform({ 0, i, 0 }));
		}
		for (u32 i = 0; i < 5; i++)
		{
			arch.destroyAtIndex(i);
		}
		arch.resolveBuffers();

		// Validate the remaining entities exist
		u32 i = 5;
		auto it = archetype::iterator<transform>({ &arch });
		auto end = archetype::iterator<transform>(0, 1);
		for (; it != end; it++)
		{
			auto [tf] = *it;
			glm::vec3 expectedPos = { 0, i, 0 };
			assert_equals(tf.pos, expectedPos);
			i++;
		}
	}
}