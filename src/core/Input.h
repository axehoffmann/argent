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
		/**
		* Register a boolean input action
		* @param action The identifying value of the action
		* @param name The name of the action
		* @param keys The keys that trigger this action
		*/
		void RegisterAction(T action, std::string& name, std::vector<int>& keys)
		{
			boolActions[action] = BoolAction(name, keys);
		}

		/**
		* Register a linear input action, value of -1 to 1
		* @param action The identifying value of the action
		* @param name The name of the action
		* @param posKeys The keys that map to a positive value of this action
		* @param negKeys The keys that map to a negative value of this action
		*/
		void RegisterAction(T action, std::string& name, std::vector<int>& posKeys, std::vector<int>& negKeys)
		{
			axisActions[action] = AxisAction(name, posKeys, negKeys);
		}

		/**
		* Modify the keys of a boolean action
		* @param action The action to modify
		* @param newKeys The new keys that map to the action
		*/
		void ModifyAction(T action, std::vector<int>& newKeys)
		{
			boolActions.at(action).keys = newKeys;
		}

		/**
		* Modify the keys of a linear input action
		* @param action The action to modify
		* @param posKeys The new keys that map to the positive value of the action
		* @param negKeys The new keys that map to the negative value of the action
		*/
		void ModifyAction(T action, std::vector<int>& posKeys, std::vector<int>& negKeys)
		{
			axisActions.at(action).positive = posKeys;
			axisActions.at(action).negative = negKeys;
		}

		/**
		* Get the value of a boolean input action
		* @param action The identifying value of the action
		* @param type The type of action eg. Press, Release
		*/
		bool GetB(T action, ActionType type)
		{
			BoolAction* action = &boolActions[action];
			for (size_t i = 0; i < action->keys.size(); i++)
			{
				if (PollFunc(action->keys[i], type))
					return true;
			}

			return false;
		}

		/**
		* Get the value of a linear input action
		* @param action The identifying value of the action
		*/
		float GetF(T action)
		{
			float negative = 0.0f;
			float positive = 0.0f;

			AxisAction* action = &axisActions[action];

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