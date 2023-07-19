#include "TestFramework.h"
#include "src/core/log/ConsoleWriter.h"

using namespace agtest;

int main()
{

	for (auto& s : agtest::Test::TestRegister::Suites())
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
		return;
	}

	result.failures.emplace_back(message, ln);
}

void PrintReportLine(int success, int fail, std::string name)
{
	std::string paddedName = name.insert(name.length(), 24 - name.length(), ' ');

	csw::print({
			{ csw::Colour::BrightWhite, "[ " },
			{ csw::Colour::White, paddedName },
			{ csw::Colour::BrightWhite, " | P: "},
			{ success > 0 ? csw::Colour::BrightGreen : csw::Colour::BrightRed, std::to_string(success) },
			{ csw::Colour::BrightWhite, "    F: "},
			{ fail == 0 ? csw::Colour::BrightGreen : csw::Colour::BrightRed, std::to_string(fail) },
			{ csw::Colour::BrightWhite, " ]" },
		});
}

std::string ToPascalCase(const std::string& in)
{
	std::string out;
	bool capsNext = true;

	for (const auto ch : in)
	{
		if (ch == '_')
		{
			capsNext = true;
			out += ' ';
			continue;
		}

		if (capsNext)
		{
			capsNext = false;
			out += static_cast<char>(std::toupper(ch));
			continue;
		}

		out += ch;
	}
	return out;
}

void agtest::Test::Run()
{
	std::vector<CaseResult> results;
	results.reserve(cases.size());

	for (Case& c : cases)
	{
		if (init) init();
		results.push_back(c.Execute());
		if (cleanup) cleanup();
	}

	int successTally = 0, failTally = 0;
	std::vector<std::string> failMsgs;

	for (CaseResult& r : results)
	{
		if (r.failures.size() == 0)
			successTally++;
		else
			failTally++;

		for (Failure& f : r.failures)
		{
			failMsgs.push_back(std::vformat("Assert fail on line {}: {}", std::make_format_args(f.lineNum, f.message)));
		}
	}

	PrintReportLine(successTally, failTally, name);
	for (std::string& fm : failMsgs)
	{
		std::cout << fm << "\n";
	}
}

agtest::Case::CaseRegister::CaseRegister(const std::string& name, const std::string& suite, void(*f)(agtest::Case*))
{
	Test::TestRegister::Suites()[suite].cases.push_back(Case(ToPascalCase(name), f));
}

agtest::Test::Test(const std::string& n, void(*initialiser)(), void(*cleaner)(), std::initializer_list<std::string> dep) : name(n), init(initialiser), cleanup(cleaner), dependencies(dep)
{

}

agtest::Test::TestRegister::TestRegister(const std::string& name, std::initializer_list<std::string> dep)
{
	Suites()[name] = Test(ToPascalCase(name), nullptr, nullptr, dep);
}

agtest::Test::TestRegister::TestRegister(const std::string& name, void(*initialiser)(), void(*cleaner)(), std::initializer_list<std::string> dependencies)
{
	Suites()[name] = Test(ToPascalCase(name), initialiser, cleaner, dependencies);
}
