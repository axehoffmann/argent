#include <string>
#include <vector>
#include <format>
#include <iostream>
#include <windows.h>

#define ag_expect(val, msg) Test::Expect(val, "line " + std::to_string(__LINE__) + ": " + msg)
#define ag_expect(val, msg, ...) Test::Expect(val, "line " + std::to_string(__LINE__) + ": " + msg, __VA_ARGS__)


class Test
{
public:
	static void Expect(bool condition, std::string failure);

	template <typename... Ins>
	static void Expect(bool condition, std::string failure, Ins... args)
	{
		Expect(condition, std::vformat(failure, std::make_format_args(args...)));
	}

	static void Case(std::string name, void (*func)());


	static void Run();
	
	static void (*init)();
	static void (*clean)();

	static void Name(std::string n) { name = n; }

private:
	struct testCase 
	{
		void (*func)();
		std::string name;

		testCase(std::string n, void (*ptr)()) : func(ptr), name(n) {}
	};


	static void InitFunc();

	static void CleanFunc();

	static int successes;
	static int failures;
	static bool failed;
	static std::vector<std::string> errorMsgs;
	static std::vector<testCase> tests;
	static std::string name;
	static std::string currentCaseName;
};