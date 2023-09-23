#include "../TestFramework.h"
#include "ecs/events/EventManager.h"

#include <thread>

namespace eventmanager_test
{
	struct EventA : ag::event::Event<EventA> 
	{
		size_t i{ 0 };
	};

	struct EventB : ag::event::Event<EventB>
	{
		bool flag{ false };
	};

	struct EventC : ag::event::Event<EventC>
	{
		std::string name{ "" };
	};


	$ITest(events, {});

	$Case(basic, events)
	{
		using namespace ag::event;
		EventManager em{};

		size_t expected = 0;
		for (size_t i = 0; i < 10; i++)
		{
			em.pushEvent<EventA>(EventA{ .i = i });
			expected += i;
		}

		for (size_t i = 0; i < 5; i++)
		{
			em.pushEvent<EventB>(EventB{ .flag = false });
		}

		size_t count = 0;
		auto tally = [&count](const EventA& event) 
		{
			count += event.i;
		};

		em.registerListener(tally);

		em.alertAll();
		ag_expect(count == expected, "Expected to read {} total value of A events, instead found {}", expected, count);

		em.clearAll();
		count = 0;
		em.alertAll();
		ag_expect(count == 0, "Expected to count 0 A's after clearing event list, instead found {}", count);
	}

	$Case(stress_test, events)
	{
		using namespace ag::event;

		EventManager em{};

		// Stress test with concurrency and multiple event types
		// Ensuring all events get sent
		std::thread one([&]()
		{
			for (size_t i = 0; i < 5000; i++)
			{
				em.pushEvent<EventA>(EventA{ .i = 1 });
			}
			for (size_t i = 0; i < 1000; i++)
			{
				em.pushEvent<EventB>(EventB{ .flag = true });
			}
		});
		std::thread two([&]()
		{
			for (size_t i = 0; i < 2000; i++)
			{
				em.pushEvent<EventC>(EventC{ .name = "" });
			}
			for (size_t i = 0; i < 2000; i++)
			{
				em.pushEvent<EventA>(EventA{ .i = 3 });
			}
		});
		std::thread three([&]()
		{
			for (size_t i = 0; i < 4000; i++)
			{
				em.pushEvent<EventC>(EventC{ .name = "" });
			}
		});
		one.join();
		two.join();
		three.join();

		int tallyA = 0;
		int tallyB = 0;
		int tallyC = 0;

		em.registerListener([&](const EventA& e)
		{
			tallyA += e.i;
		});
		em.registerListener([&](const EventB& e)
		{
			tallyA += e.flag ? 1 : 0;
		});
		em.registerListener([&](const EventB& e)
		{
			tallyA += 1;
		});

		em.alertAll();

		ag_expect(tallyA == 11000, "Expected total A tally of 11000, instead found {}", tallyA);
		ag_expect(tallyB == 11000, "Expected total B tally of 1000, instead found {}", tallyB);
		ag_expect(tallyC == 11000, "Expected total C tally of 6000, instead found {}", tallyC);

		em.clearAll();
	}
}