#pragma once

#include <vector>
#include <string>
#include <debug/log/Log.h>

#define $utest(tsuite) static tf::t_suite tsuite(std::source_location::current())

namespace tf
{

	struct context
	{
		std::vector<std::string> failures;

		void dump()
		{
			for (const std::string& s : failures)
			{
				std::cout << s << std::endl;
			}
		}
	};

	template <typename T>
	void assert_equals(context& ctx, T a, T b, std::source_location loc = std::source_location::current())
	{
		if (a == b) return;
		ctx.failures.push_back(std::format("Assertion Fail ({}): {} != {}", loc.line()));
	}

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
		std::vector<t_case*> cases;

		t_suite(std::source_location loc)
			: name(loc.file_name()) 
		{
			reg().emplace_back(this);
		}

		void run()
		{
			context ctx;
			for (t_case* c : cases)
				c->func(ctx);

			ctx.dump();
		}

		static std::vector<t_suite*> reg()
		{
			std::vector<t_suite*> r;
			return r;
		}
	};
}