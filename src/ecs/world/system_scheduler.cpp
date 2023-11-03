#include "system_scheduler.h"

using namespace ag;

auto build_system_schedule() -> ptr<job_graph<isystem>>
{
	return make<job_graph<isystem>>();
}
