#include "test_framework.h"

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