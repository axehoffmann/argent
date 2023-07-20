#pragma once

#include <iostream>
#include <string>
#include <vector>

class csw
{
public:
	enum class Colour : uint16_t
	{
		Black = 30,
		Red = 31,
		Green = 32,
		Yellow = 33,
		Blue = 34,
		Magenta = 35,
		Cyan = 36,
		White = 37,
		BrightBlack = 90,
		BrightRed = 91,
		BrightGreen = 92,
		BrightYellow = 93,
		BrightBlue = 94,
		BrightMagenta = 95,
		BrightCyan = 96,
		BrightWhite = 97,
		Unset
	};

	static std::string ColourCode(uint16_t c)
	{
		return "\x1B[" + std::to_string(c) + "m";
	}

	struct ColourSegment
	{
		ColourSegment(Colour c, std::string t) : col((uint16_t)c), text(t), bgcol((uint16_t)Colour::Unset) {}
		ColourSegment(Colour c, Colour bgc, std::string t) : col((uint16_t)c), bgcol((uint16_t)bgc), text(t) {}
		uint16_t col;
		uint16_t bgcol;
		std::string text;

		std::string tostr() const
		{
			if (bgcol == (uint16_t)Colour::Unset)
				return ColourCode(col) + text;

			return ColourCode(col) + ColourCode(bgcol + 10) + text;
		}
	};

	using colour_text = std::vector<ColourSegment>;

	static std::string RawText(const colour_text& text)
	{
		std::string out = "";
		for (const ColourSegment& cs : text)
		{
			out += cs.text;
		}
		return out;
	}

	static std::string ColourText(const colour_text& text)
	{
		std::string out = "";
		for (const ColourSegment& cs : text)
		{
			out += cs.tostr();
		}
		return out;
	}

	static void print(const colour_text& text)
	{
		for (const ColourSegment& cs : text)
		{
			std::printf(cs.tostr().c_str());
		}

		std::printf("\n");
	}
};