#include "tests/test_framework.h"
#include "resources/v2/packer/mesh.h"

namespace meshpk
{
	const string mesh_path = "assets/cube.obj";
	
	$utest(mesh_pack);

	$tcase(load, mesh_pack)
	{
		auto cube = load_obj(mesh_path);

		assert_equals(cube.indices.size(), u64{36});

		auto m2 = optimize_mesh(std::move(cube));
	}
}