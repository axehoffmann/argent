#include "TestFramework.h"
void(*Test::init)() = nullptr;
void(*Test::clean)() = nullptr;
int Test::successes(0);
int Test::failures(0);
bool Test::failed(false);
std::vector<std::string> Test::errorMsgs;
std::vector<Test::testCase> Test::tests;
std::string Test::name;
std::string Test::currentCaseName;

void Test::Expect(bool condition, std::string failure)
{
	if (!condition)
	{
		failed = true;
		errorMsgs.push_back(currentCaseName + " error on " + failure);
	}
}

void Test::Case(std::string name, void(*func)())
{
	tests.push_back(testCase(name, func));
}

void Test::Run()
{
	for (size_t i = 0; i < tests.size(); i++)
	{
		currentCaseName = tests[i].name;
		InitFunc();
		tests[i].func();
		CleanFunc();

		if (failed)
			failures++;
		else
			successes++;
		
		failed = false;
	}

	HANDLE c = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(c, 15 + 0 * 16);
	std::cout << "[ ";
	SetConsoleTextAttribute(c, 7 + 0 * 16);
	std::cout << name;
	SetConsoleTextAttribute(c, 15 + 0 * 16);
	std::cout << " | Passed: ";
	SetConsoleTextAttribute(c, successes > 0 ? 10 : 12);
	std::cout << std::to_string(successes);
	SetConsoleTextAttribute(c, 15 + 0 * 16);
	std::cout << " Failed: ";
	SetConsoleTextAttribute(c, failures == 0 ? 10 : 12);
	std::cout << std::to_string(failures);
	SetConsoleTextAttribute(c, 15 + 0 * 16);
	std::cout << " ]" << std::endl;

	for (size_t i = 0; i < errorMsgs.size(); i++)
	{
		std::cout << errorMsgs[i] << std::endl;
	}
}

void Test::InitFunc()
{
	if (init)
		init();
}

void Test::CleanFunc()
{
	if (clean)
		clean();
}