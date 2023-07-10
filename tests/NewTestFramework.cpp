#include "NewTestFramework.h"

using namespace agtest;

int main()
{
	for (auto& s : agtest::Suite::SuiteRegister::Suites())
	{
		s.second.Run();
	}
}

CaseResult Case::Execute()
{
	func(this);
	return result;
}

void agtest::Case::Expect(bool condition, int ln, std::string message)
{
	if (condition)
	{
		std::cout << "success\n";
		return;
	}
	std::cout << "fail\n";

	result.failures.emplace_back(message, ln);
}

void agtest::Suite::Run()
{
	for (Case& c : cases)
	{
		if (init) init();
		c.Execute();
		if (cleanup) cleanup();
	}
}

agtest::Case::CaseRegister::CaseRegister(const std::string& name, const std::string& suite, void(*f)(agtest::Case*))
{
	Suite::SuiteRegister::Suites()[suite].cases.push_back(Case(name, f));
}

agtest::Suite::Suite(const std::string& n, void(*initialiser)(), void(*cleaner)(), std::initializer_list<std::string> dep) : name(n), init(initialiser), cleanup(cleaner), dependencies(dep)
{

}

agtest::Suite::SuiteRegister::SuiteRegister(const std::string& name, std::initializer_list<std::string> dep)
{
	Suites()[name] = Suite(name, nullptr, nullptr, dep);
}

agtest::Suite::SuiteRegister::SuiteRegister(const std::string& name, void(*initialiser)(), void(*cleaner)(), std::initializer_list<std::string> dependencies)
{
	Suites()[name] = Suite(name, initialiser, cleaner, dependencies);
}
