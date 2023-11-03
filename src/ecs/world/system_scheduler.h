#pragma once

#include "query.h"
#include "system.h"

#include "lib/ptr.h"
#include "lib/job_graph.h"

namespace ag
{
	auto build_system_schedule() -> ptr<job_graph<ag::isystem>>;
}
