#pragma once

#define GLEW_STATIC
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <unordered_map>
namespace ag
{
	enum class ActionType
	{
		Press, /* True on frame button is pressed */
		Release, /* True on frame button is released */
		Down, /* True while button is held */
		Up /* True while button isn't held */
	};

	class Input
	{
	public:
		void RegisterAction(std::string& name, std::vector<int>& keys);
		void RegisterAction(std::string& name, std::vector<int>& posKeys, std::vector<int>& negKeys);

		bool GetB(std::string& name, ActionType type);
		float GetF(std::string& name);

	private:
		struct BoolAction
		{
			std::vector<int> keys;

			BoolAction(std::vector<int>& k) { keys = k; }
		};
		
		struct AxisAction
		{
			std::vector<int> positive;
			std::vector<int> negative;

			AxisAction(std::vector<int>& p, std::vector<int>& n) { positive = p; negative = n; }
		};

		bool PollFunc(int key, ActionType type);

		std::unordered_map<std::string, BoolAction> boolActions;
		std::unordered_map<std::string, AxisAction> axisActions;
	};
}