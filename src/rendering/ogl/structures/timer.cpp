#include "timer.h"

#include <algorithm>

gltimer::gltimer(function<f32> cb) :
	callback(std::move(cb))
{
	glCreateQueries(GL_TIME_ELAPSED, 1, &id);
}

gltimer::~gltimer()
{
	glDeleteQueries(1, &id);
}

void gltimer::start()
{
	i32 value = 0;
	glGetQueryObjectiv(id, GL_QUERY_RESULT_NO_WAIT, &value);
	if (value != 0)
	{
		values.push_back(value / 1e6);
	}
	if (values.size() >= 100)
	{
		callback(std::reduce(values.begin(), values.end()) / values.size());
		values.clear();
		values.reserve(100);
	}

	glBeginQuery(GL_TIME_ELAPSED, id);
}

void gltimer::stop()
{
	glEndQuery(GL_TIME_ELAPSED);
}
