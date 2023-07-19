#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <format>
#include <source_location>
#include "ConsoleWriter.h"

namespace ag
{
	class LogImpl
	{
		template <class T> friend class Log;
	private:
		std::vector<csw::colour_text> buffer;

		static void Write(const std::vector<csw::ColourSegment>& text)
		{
			std::cout << csw::ColourText(text) << std::endl;
		}

		static const std::string repository_name;
		static std::string cut_fluff_directory(const std::string& path)
		{
			size_t index = path.find(repository_name);
			if (index == std::string::npos)
				return path;

			return path.substr(index + repository_name.size(), path.size());
		}
	};

	template <class T>
	class Log
	{
	public:
		void Trace(const std::string& message, 
			const std::source_location loc = std::source_location::current())
		{

			std::string filename = LogImpl::cut_fluff_directory(loc.file_name());
			LogImpl::Write({ 
					{ csw::Colour::BrightWhite, "["				},
					{ csw::Colour::BrightCyan,	"TRACE"			},
					{ csw::Colour::BrightWhite, "] "			},
					{ csw::Colour::BrightWhite, filename		},
					{ csw::Colour::White,		"(ln: "			},
					{ csw::Colour::BrightWhite, std::to_string(loc.line())		},
					{ csw::Colour::White,		") "			},
					{ csw::Colour::White,		message			}
				});
		}
		template <typename... Ts>
		void Trace(const std::string& message, 
			const std::source_location loc = std::source_location::current(), 
			Ts... formatArgs)
		{
			Trace(std::vformat(message, std::make_format_args(formatArgs...)), loc);
		}
	};
}