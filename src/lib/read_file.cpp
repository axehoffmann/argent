#include "read_file.h"

#include <fstream>
#include <sstream>

#include "debug/log/Log.h"

string readText(const string& path)
{
	string source;
	std::ifstream stream(path);
	if (stream.is_open())
	{
		std::stringstream s;
		s << stream.rdbuf();
		source = s.str();
		stream.close();
	}
	else
	{
		ag::Log::Error("Failed to open " + path);
		return "";
	}

	return source;
}