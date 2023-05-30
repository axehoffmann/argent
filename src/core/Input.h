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

	template <typename T>
	class Input
	{
	public:
		void RegisterAction(T key, std::string& name, std::vector<int>& keys)
		{
			boolActions[key] = BoolAction(name, keys);
		}

		void RegisterAction(T key, std::string& name, std::vector<int>& posKeys, std::vector<int>& negKeys)
		{
			axisActions[key] = AxisAction(name, posKeys, negKeys);
		}

		bool GetB(T key, ActionType type)
		{
			BoolAction* action = &boolActions[key];
			for (size_t i = 0; i < action->keys.size(); i++)
			{
				if (PollFunc(action->keys[i], type))
					return true;
			}

			return false;
		}

		float GetF(T key)
		{
			float negative = 0.0f;
			float positive = 0.0f;

			AxisAction* action = &axisActions[key];

			for (size_t i = 0; i < action->positive.size(); i++)
			{
				if (PollFunc(action->positive[i], ActionType::Down))
				{
					positive = 1.0f;
					break;
				}
			}

			for (size_t i = 0; i < action->negative.size(); i++)
			{
				if (PollFunc(action->negative[i], ActionType::Down))
				{
					negative = 1.0f;
					break;
				}
			}

			return positive - negative;
		}

	private:
		struct BoolAction
		{
			std::string name;
			std::vector<int> keys;

			BoolAction(std::string n, std::vector<int>& k) { name = n; keys = k; }
		};
		
		struct AxisAction
		{
			std::string name;
			std::vector<int> positive;
			std::vector<int> negative;

			AxisAction(std::string n, std::vector<int>& p, std::vector<int>& ne) { name = n; positive = p; negative = ne; }
		};

		bool PollFunc(int key, ActionType type);

		std::unordered_map<T, BoolAction> boolActions;
		std::unordered_map<T, AxisAction> axisActions;
	};
}