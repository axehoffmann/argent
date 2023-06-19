#include "CVARs.h"

std::vector<ag::cvars::ICVAR*> ag::CVARs::vars{};

void ag::CVARs::LoadFile(std::string path)
{
	using json = nlohmann::json;

	Cleanup();

	json conf = ag::AssetManager::ReadJson(path);
	for (json::iterator it = conf.begin(); it != conf.end(); it++)
	{
		if ((*it)["type"] == "float")
			Create<float>((*it)["name"], (*it)["desc"], (float)(*it)["val"]);
		else if ((*it)["type"] == "int")
			Create<int>((*it)["name"], (*it)["desc"], (int)(*it)["val"]);
		else if ((*it)["type"] == "bool")
			Create<bool>((*it)["name"], (*it)["desc"], (bool)(*it)["val"]);
		else if ((*it)["type"] == "string")
			Create<std::string>((*it)["name"], (*it)["desc"], (std::string)(*it)["val"]);
	}
}