#include "Stats.h"

std::array<double, 10> ag::Stats::frameTimeLog{};
int ag::Stats::frameLogIndex(0);

void ag::Stats::RegisterFrameTime(double frameTime)
{
	frameTimeLog[frameLogIndex] = frameTime;
	frameLogIndex++;
	frameLogIndex %= frameTimeLog.size();
}

float ag::Stats::GetAverageFrameTime()
{
	double total = 0.0;
	for (size_t i = 0; i < frameTimeLog.size(); i++)
	{
		total += frameTimeLog[i];
	}
	return (float)(total / frameTimeLog.size() * 1000.0f);
}
