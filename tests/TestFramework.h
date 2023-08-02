#pragma once

#include <string>
#include <vector>
#include <format>
#include <iostream>
#include <unordered_map>
#include <windows.h>

#define INTERNAL_CONCAT__(a, b) a##b
#define INTERNAL_CONCAT(a, b) INTERNAL_CONCAT__(a, b)

#define INTERNALSWITCH(_1, _2, _3, _4, NAME,...) NAME
#define INTERNALSuite_1(name) static agtest::Test::TestRegister name(#name, {})
#define INTERNALSuite_2(name, dependencies) static agtest::Test::TestRegister name(#name, dependencies)
#define INTERNALSuite_3(name, init, cleanup) static agtest::Test::TestRegister name(#name, init, cleanup, {})
#define INTERNALSuite_4(name, init, cleanup, dependencies) static agtest::Test::TestRegister name(#name, init, cleanup, dependencies)

#define $UTest(name) static agtest::Test::TestRegister name(#name, {})
#define $ITest(name, dependencies) static agtest::Test::TestRegister name(#name, dependencies)

#define $Init(test)															\
	void INTERNAL_CONCAT(test, _init)();										\
	static agtest::Test::FuncRegister internal_init_##test(0, INTERNAL_CONCAT(test, _init), #test);	\
	void INTERNAL_CONCAT(test, _init)()

#define $Cleanup(test)															\
	void INTERNAL_CONCAT(test, _cleanup)();										\
	static agtest::Test::FuncRegister internal_cleanup_##test(1, INTERNAL_CONCAT(test, _cleanup), #test);	\
	void INTERNAL_CONCAT(test, _cleanup)()


#define $Test(...) INTERNALSWITCH(__VA_ARGS__, INTERNALSuite_4, INTERNALSuite_3, INTERNALSuite_2, INTERNALSuite_1)(__VA_ARGS__)

#define FUNCNAME(NAME, SUITE) INTERNAL_CONCAT(func_##SUITE, _##NAME)
#define $Case(NAME, suite)														\
	void FUNCNAME(NAME,suite)(agtest::Case* case__hide);									\
	static agtest::Case::CaseRegister NAME(#NAME, #suite, FUNCNAME(NAME,suite));			\
	void FUNCNAME(NAME,suite)(agtest::Case* case__hide)


#define ag_expect(val, ...) case__hide->Expect(val, __LINE__, __VA_ARGS__)


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
			Expect(condition, ln, std::vformat(message, std::make_format_args(params...)));
		}

		class CaseRegister
		{
		public:
			CaseRegister(const std::string& name, const std::string& suite, void(*f)(Case*));
		};
	};

	class Test
	{
	public:
		std::string name;
		std::vector<Case> cases;
		std::vector<std::string> dependencies;

		void (*init)();
		void (*cleanup)();

		void Run();

		Test() : name("example"), init(nullptr), cleanup(nullptr) {}
		Test(const std::string& n, void(*initialiser)(), void(*cleaner)(), std::initializer_list<std::string> dep);

		class TestRegister
		{
		public:
			static std::unordered_map<std::string, Test>& Suites()
			{
				static std::unordered_map<std::string, Test> s;
				return s;
			}

			TestRegister(const std::string& name, std::initializer_list<std::string> dep);
			TestRegister(const std::string& name, void(*initialiser)(), void(*cleaner)(), std::initializer_list<std::string> dependencies);
		};

		class FuncRegister
		{
		public:
			FuncRegister(int type, void(*func)(), const std::string& test)
			{
				if (type == 0)
					TestRegister::Suites()[test].init = func;
				else
					TestRegister::Suites()[test].cleanup = func;
			}
		};
			
	};
}
