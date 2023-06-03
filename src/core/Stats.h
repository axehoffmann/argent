#pragma once

#include <array>

namespace ag
{
	class Stats
	{
	public:
		/**
		* Logs the frame time of the most recent frame
		*/
		static void RegisterFrameTime(double frameTime);
		/**
		* Gets the average frame time of the last 10 frames
		*/
		static float GetAverageFrameTime();
	private:
		static std::array<double, 10> frameTimeLog;
		static int frameLogIndex;
	};
}