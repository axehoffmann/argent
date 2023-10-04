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
	private:
		static std::vector<csw::colour_text> buffer;
		static const std::string repository_name;

	public:
		static inline bool buffered = false;
		static void Write(const std::vector<csw::ColourSegment>& text, bool endl = true)
		{
			buffer.push_back(text);

			if (!buffered)
				Flush();
			/*
			if (endl)
				std::cout << std::endl;
			else
				std::cout << std::flush;
			*/
		}

		static std::string cut_fluff_directory(const std::string& path)
		{
			size_t index = path.find(repository_name);
			if (index == std::string::npos)
				return path;

			return path.substr(index + repository_name.size(), path.size());
		}

		static void Flush()
		{
			for (auto& text : buffer)
			{
				std::cout << csw::ColourText(text) << "\n";
			}
			buffer.clear();
		}

		static void Clear()
		{
			buffer.clear();
		}
	};

	/**
	* Shorthand for formatting a string
	*/
	template <typename... Ts>
	std::string sfmt(const std::string& message, Ts... formatArgs)
	{
		return std::vformat(message, std::make_format_args(formatArgs...));
	}

	class Log
	{
	private:
		static void LogMessage(const std::string& message,
			const std::string& logType,
			csw::Colour logTypeColour,
			const std::source_location loc)
		{

			std::string filename = LogImpl::cut_fluff_directory(loc.file_name());
			LogImpl::Write({
					{ csw::Colour::BrightWhite, " ["						},
					{ logTypeColour,			logType						},
					{ csw::Colour::BrightWhite, "] "						},
					{ csw::Colour::BrightWhite, filename					},
					{ csw::Colour::BrightBlack,	"(ln: "						},
					{ csw::Colour::BrightWhite, std::to_string(loc.line())  },
					{ csw::Colour::BrightBlack,	") "						},
					{ csw::Colour::White,		message						}
				});
		}
	public:
		static void Trace(const std::string& message, 
			const std::source_location loc = std::source_location::current())
		{
			LogMessage(message, "TRACE", csw::Colour::BrightCyan, loc);
		}

		static void Warn(const std::string& message,
			const std::source_location loc = std::source_location::current())
		{
			LogMessage(message, "WARN", csw::Colour::BrightYellow, loc);
		}

		static void Error(const std::string& message,
			const std::source_location loc = std::source_location::current())
		{
			LogMessage(message, "ERROR", csw::Colour::BrightRed, loc);
		}

		static void Flush()
		{
			LogImpl::Flush();
		}

		static void ClearBuffer()
		{
			LogImpl::Clear();
		}

		static void SetBuffered(bool b)
		{
			LogImpl::buffered = b;
		}
		/*
		class Progress
		{
		public:
			Progress(const std::string& t, float m) : 
				title(t), max(m), progress(0.0f), width(20) 
			{ Update(0); }
			void Update(float prog)
			{
				progress = prog;
				std::string titleStr = "  " + title;
				int titlePadding = title.size() > 16 ? 0 : 16 - title.size();
				titleStr.insert(titleStr.end(), titlePadding + 2, ' ');

				std::string bar = "";
				float ratio = (progress / max);
				int fullAmount = ratio * width;
				bar.insert(bar.end(), fullAmount, '#');
				bar.insert(bar.end(), width - fullAmount, '-');

				//csw::Colour barCol = ratio < 0.3f ? csw::Colour::Yellow :
				//					 ratio < 0.5f ? csw::Colour::BrightYellow :
				//					 ratio < 0.75f ? csw::Colour::BrightGreen :
				//					 csw::Colour::Green;
									 

				std::string ratioStr = sfmt("{}/{}", (int)progress, (int)max);

				LogImpl::Write({
						{ csw::Colour::BrightCyan,	titleStr },
						{ csw::Colour::BrightBlack,	" [ " },
						{ csw::Colour::BrightWhite,	bar },
						{ csw::Colour::BrightBlack,	" ] " },
						{ csw::Colour::BrightWhite, ratioStr },
						{ csw::Colour::BrightWhite, "\t\r" }
					}, false);
			}

		private:

			std::string title;
			float max;
			float progress;
			int width;
		};
		*/

 	};
}