#pragma once

#include <array>

namespace ag
{
	class Stats
	{
	public:
		static void RegisterFrameTime(double frameTime);
		static float GetAverageFrameTime();
	private:
		static std::array<double, 10> frameTimeLog;
		static int frameLogIndex;
	};
}