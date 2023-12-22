#include "lib/basic.h"

#include "mesh.h"
#include "tex.h"

/**
* Command-line tool for processing assets
*/
auto main() -> i32
{
	compile_mesh("assets/pillar/pillar.obj");
	compile_mesh("assets/cube.obj");
	pack_texture_rgb("assets/pillar/pillar_detail.png", "assets/pillar/pillar_rough.png","","");
}