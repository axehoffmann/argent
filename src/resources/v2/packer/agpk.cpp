#include "lib/basic.h"

#include "mesh.h"

/**
* Command-line tool for processing assets
*/
auto main() -> i32
{
	compile_mesh("assets/pillar/pillar.obj");
	compile_mesh("assets/cube.obj");
}