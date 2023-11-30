#include "test_framework.h"

std::string get_fname(const std::string& path)
{
	return path.substr(path.find_last_of('\\') + 1, path.length());
}

tf::t_case::t_case(std::string n, t_suite* s, case_func f) :
	name(n),
	func(f)
{
	s->cases.push_back(this);
}

int main()
{
	for (auto s : tf::t_suite::reg())
	{
		s->run();
	}
}