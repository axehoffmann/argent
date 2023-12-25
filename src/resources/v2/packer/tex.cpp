#include "tex.h"

#include "debug/log/Log.h"

#pragma warning(push, 0)
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#pragma warning(pop)


#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(pop)

tex load_tex(const string& path)
{
	tex t{};
	i32 c;
	t.data = stbi_load(path.c_str(), &t.w, &t.h, &c, STBI_rgb);
	std::cout << c << '\n';
	return t;
}

void pack_texture_rgb(const string& out, const string& r, const string& g, const string& b)
{
	tex rt = load_tex(r);
	tex gt = load_tex(g);
	/*
	tex bt = load_tex(b);

	if (rt.h != gt.h || gt.h != bt.h ||
		rt.w != gt.w || gt.w != bt.w)
		ag::Log::Error(ag::sfmt("Failed to pack texture {}, input texture dimensions were not equal", out));
	*/
	vector<u8> ot(u64{3} * rt.w * rt.h);
	
	for (u64 i = 0; i < rt.w * rt.h; i++)
	{
		ot[i * 3] = rt.data[i * 3];
		ot[i * 3 + 1] = gt.data[i * 3 + 1];
	}

	stbi_image_free(rt.data);
	stbi_image_free(gt.data);

	stbi_write_png(out.c_str(), rt.w, rt.h, STBI_rgb, ot.data(), rt.w * 3);
}
