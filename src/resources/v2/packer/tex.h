#pragma once

#include "lib/basic.h"
#include "lib/string.h"
#include "lib/vector.h"

struct tex
{
	u8* data;
	i32 w;
	i32 h;
};

tex load_tex(const string& path);
void pack_texture_rgb(const string& out, const string& r, const string& g, const string& b);