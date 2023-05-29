#include "Input.h"

void ag::Input::RegisterAction(std::string& name, std::vector<int>& keys)
{
	boolActions[name] = BoolAction(keys);
}

void ag::Input::RegisterAction(std::string& name, std::vector<int>& posKeys, std::vector<int>& negKeys)
{
	axisActions[name] = AxisAction(posKeys, negKeys);
}

/// TODO: Error on invalid action name
bool ag::Input::GetB(std::string& name, ActionType type)
{
	BoolAction* action = &boolActions[name];
	for (size_t i = 0; i < action->keys.size(); i++)
	{
		if (PollFunc(action->keys[i], type))
			return true;
	}

	return false;
}

/// TODO: Error on invalid action name
float ag::Input::GetF(std::string& name)
{
	float negative = 0.0f;
	float positive = 0.0f;

	AxisAction* action = &axisActions[name];

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
