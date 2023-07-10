#include <string>
#include <vector>
#include <format>
#include <iostream>
#include <unordered_map>
#include <windows.h>


#define INTERNALSWITCH(_1, _2, _3, _4, NAME,...) NAME
#define INTERNALSuite_1(name) static agtest::Suite::SuiteRegister name(#name, {})
#define INTERNALSuite_2(name, dependencies) static agtest::Suite::SuiteRegister name(#name, dependencies)
#define INTERNALSuite_4(name, init, cleanup, dependencies) static agtest::Suite::SuiteRegister name(#name, init, cleanup, dependencies)

#define $Suite(...) INTERNALSWITCH(__VA_ARGS__, INTERNALSuite_4, ERROR_3_ARGUMENT_INVALID, INTERNALSuite_2, INTERNALSuite_1)(__VA_ARGS__)

#define $Case(NAME, suite)														\
	void func_##NAME(agtest::Case* case__hide);									\
	static agtest::Case::CaseRegister NAME(#NAME, #suite, func_##NAME);			\
	void func_##NAME(agtest::Case* case__hide)


#define ag_expect(val, msg, ...) case__hide->Expect(val, __LINE__, msg, __VA_ARGS__)

namespace agtest
{
	struct Failure
	{
		std::string message;
		int lineNum;

		Failure(const std::string& s, int ln) : lineNum(ln), message(s) {}
	};

	struct CaseResult
	{
		std::vector<Failure> failures;
	};

	class Case
	{
	public:
		void (*func)(Case*);
		std::string name;

		CaseResult result;

		Case(std::string n, void (*ptr)(Case*)) : func(ptr), name(n) {}

		CaseResult Execute();

		void Expect(bool condition, int ln, std::string message);

		template <typename... Ts>
		void Expect(bool condition, int ln, std::string message, Ts... params)
		{
			Expect(condition, std::vformat(message, std::make_format_args(params...)));
		}

		class CaseRegister
		{
		public:
			CaseRegister(const std::string& name, const std::string& suite, void(*f)(Case*));
		};
	};

	class Suite
	{
	public:
		std::string name;
		std::vector<Case> cases;
		std::vector<std::string> dependencies;

		void (*init)();
		void (*cleanup)();

		void Run();

		Suite() : name("example"), init(nullptr), cleanup(nullptr) {}
		Suite(const std::string& n, void(*initialiser)(), void(*cleaner)(), std::initializer_list<std::string> dep);

		class SuiteRegister
		{
		public:
			static std::unordered_map<std::string, Suite>& Suites()
			{
				static std::unordered_map<std::string, Suite> s;
				return s;
			}
			SuiteRegister(const std::string& name, std::initializer_list<std::string> dep);
			SuiteRegister(const std::string& name, void(*initialiser)(), void(*cleaner)(), std::initializer_list<std::string> dependencies);
		};
	};
}