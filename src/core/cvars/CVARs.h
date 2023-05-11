#include <string>
#include <vector>
#include <memory>

#include "CVAR.h"

namespace ag
{

	class CVARs
	{
		template <typename T>
		static void Create(std::string name, std::string desc, T defaultVal)
		{
			cvars::CVAR<T>* var = new cvars::CVAR<T>(name, desc, defaultVal);
			vars.push_back(var);
		}

		template <typename T>
		static std::weak_ptr<T> Get(std::string name)
		{
			size_t id = cvars::ICVAR::GetID<T>();
			for (size_t i = 0; i < vars.size(); i++)
			{
				if (id == vars.at(i)->type && name == vars.at(i)->name)
				{
					cvars::CVAR<T>* var = dynamic_cast<cvars::CVAR<T>*>(vars.at(i));
					return std::weak_ptr(var);
				}
			}
		}

		template <typename T>
		static void Set(std::string name, T val)
		{
			size_t id = cvars::ICVAR::GetID<T>();
			for (size_t i = 0; i < vars.size(); i++)
			{
				if (id == vars.at(i)->type && name == vars.at(i)->name)
				{
					cvars::CVAR<T>* var = dynamic_cast<cvars::CVAR<T>*>(vars.at(i));
					var->data = val;
				}
			}
		}

		static void Cleanup()
		{
			for (size_t i = 0; i < vars.size(); i++)
			{
				delete vars[i];
			}

			std::vector<cvars::ICVAR*>().swap(vars);
		}

	private:
		static std::vector<cvars::ICVAR*> vars;
	};
}