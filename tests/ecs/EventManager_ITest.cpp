#include "../TestFramework.h"
#include "ecs/events/EventManager.h"

namespace eventmanager_test
{
	struct TestEvent : ag::event::Event<TestEvent> {
		size_t i{ 0 };
	};

	$ITest(events, {});

	$Case(basic, events)
	{
		using namespace ag::event;
		EventManager em{};

		for (size_t i = 0; i < 10; i++)
		{
			em.pushEvent<TestEvent>(TestEvent{ .i = i });
		}

		int count = 0;
		em.readEvents([&count](const TestEvent& event) {
			count++;
		});
		ag_expect(count == 10, "Expected to read 10 events, instead found {}", count);

	}
}