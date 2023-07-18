#include <iostream>
#include <string>
#include <vector>

namespace csw
{
	enum class Colour : uint16_t
	{
		Black         = 30,
		Red           = 31,
		Green         = 32,
		Yellow        = 33,
		Blue          = 34,
		Magenta       = 35,
		Cyan          = 36,
		White         = 37,
		BrightBlack   = 90,
		BrightRed     = 91,
		BrightGreen   = 92,
		BrightYellow  = 93,
		BrightBlue    = 94,
		BrightMagenta = 95,
		BrightCyan    = 96,
		BrightWhite   = 97
	};

	std::string ColourCode(uint16_t c)
	{
		return "\x1B[" + std::to_string(c) + "m";
	}

	struct ColourSegment
	{
		ColourSegment(Colour c, std::string t) : col((uint16_t)c), text(t), bgcol(30) {}
		ColourSegment(Colour c, Colour bgc, std::string t) : col((uint16_t)c), bgcol((uint16_t)bgc), text(t) {}
		uint16_t col;
		uint16_t bgcol;
		std::string text;

		std::string tostr() const
		{
			return ColourCode(col) + ColourCode(bgcol + 10) + text;
		}
	};

	void print(const std::vector<ColourSegment>& text)
	{
		for (const ColourSegment& cs : text)
		{
			std::printf(cs.tostr().c_str());
		}

		std::printf("\n");
	}

}