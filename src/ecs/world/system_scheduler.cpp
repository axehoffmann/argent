#include "system_scheduler.h"

using namespace ag;

auto build_system_schedule() -> ptr<job_graph<isystem>>
{
	/// TODO: adjust so readonlys can be parrallelised

	auto& systems = ag::isystem::registers();

	return make<job_graph<isystem>>();
}
