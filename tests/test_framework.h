#pragma once

#include <vector>
#include <string>
#include <debug/log/Log.h>

namespace tf
{
	class context
	{
	public:
		std::vector<std::string> failures;
	};

	using case_func = void(*)(context&);

	class t_case
	{
	public:
		std::string name;
		case_func func;
	};

	class t_suite
	{
	public:
		std::string name;
		std::vector<t_case> cases;

		void run()
		{

		}
	};
}