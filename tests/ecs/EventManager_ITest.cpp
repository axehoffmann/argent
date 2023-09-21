#include "../TestFramework.h"
#include "ecs/events/EventManager.h"

namespace eventmanager_test
{
	struct TestEvent : ag::event::Event<TestEvent> 
	{
		size_t i{ 0 };
	};

	$ITest(events, {});

	$Case(basic, events)
	{
		using namespace ag::event;
		EventManager em{};

		size_t expected = 0;
		for (size_t i = 0; i < 10; i++)
		{
			em.pushEvent<TestEvent>(TestEvent{ .i = i });
			expected += i;
		}

		size_t count = 0;
		auto tally = [&count](const TestEvent& event) 
		{
			count += event.i;
		};

		em.registerListener(tally);

		em.alertAll();
		ag_expect(count == expected, "Expected to read {} total value of events, instead found {}", expected, count);

		em.clearAll();
		count = 0;
		em.alertAll();
		ag_expect(count == 0, "Expected to count 0 after clearing event list, instead found {}", count);
	}
}