#pragma once

#include <vector>
#include <string>
#include <format>
#include <debug/log/Log.h>

#define INTERNAL_CONCAT__(a, b) a##b
#define INTERNAL_CONCAT(a, b) INTERNAL_CONCAT__(a, b)
#define FUNCNAME(CASE, SUITE) INTERNAL_CONCAT(func_##SUITE, _##CASE)

#define $utest(tsuite) static tf::t_suite tsuite(std::source_location::current())

#define $tcase(tcase, tsuite)									\
	void FUNCNAME(tcase, tsuite)(tf::context& ctx);				\
	static tf::t_case tcase(#tcase, &tsuite, FUNCNAME(tcase, tsuite));	\
	void FUNCNAME(tcase, tsuite)(tf::context& ctx)

#define assert_equals(a, b) tf::assertEquals(ctx, a, b, std::source_location::current())

std::string get_fname(const std::string& path);

namespace tf
{
	struct context
	{
		std::vector<std::string> failures;

		void dump()
		{
			for (const std::string& s : failures)
			{
				std::cout << col::brightblack << "   |- " << s << '\n';
			}

			if (failures.size() > 0)
				std::cout << '\n';
		}
	};

	template <typename T>
	void assertEquals(context& ctx, T a, T b, std::source_location loc = std::source_location::current())
	{
		if (a == b) return;
		ctx.failures.push_back(std::vformat(col::brightwhite +"Assertion Fail (ln "+ col::brightcyan +"{}"+ col::white +"): {} "+ col::brightred +"!= "+ col::white +"{} \n", std::make_format_args(loc.line(), a, b)));
	}

	using case_func = void(*)(context&);

	class t_suite;
	class t_case
	{
	public:
		t_case(std::string n, t_suite* s, case_func f);
		std::string name;
		case_func func;
	};

	class t_suite
	{
	public:
		std::string name;
		std::vector<t_case*> cases;

		t_suite(std::source_location loc)
			: name(get_fname(loc.file_name()))
		{
			reg().emplace_back(this);

			// add padding to the name
			name.insert(name.length(), 28 - name.length(), ' ');
		}

		void run()
		{
			context ctx;
			for (t_case* c : cases)
				c->func(ctx);

			if (ctx.failures.size() == 0)
				std::cout << col::brightblack << "  [ " << col::brightwhite << name << col::brightblack << " ]   [ " << col::brightgreen <<"PASS" << col::brightblack <<" ] \n";
			else
				std::cout << col::brightblack << "  [ " << col::brightwhite << name << col::brightblack << " ]   [ " << col::brightred <<"FAIL" << col::brightblack <<" ] \n";

			ctx.dump();
		}

		static std::vector<t_suite*>& reg()
		{
			static std::vector<t_suite*> r;
			return r;
		}
	};
}