#pragma once

#include "lib/string.h"
#include "lib/tuple.h"
#include "lib/vector.h"

#include "rendering/glm.h"

#include "meshoptimizer.h"

#include <bitsery/bitsery.h>
#include <bitsery/traits/vector.h>


struct basic_vertex
{
	f32 x, y, z;
	f32 u, v;
	f32 nx, ny, nz;
};

struct mesh_ir
{
	vector<basic_vertex> vertices;
	vector<u32> indices;
};

template <typename S>
void serialize(S& s, mesh_ir& o)
{
	s.container(o.vertices, 1 << 24, [](S& s, basic_vertex& v) 
	{
		s.value4b(v.x);
		s.value4b(v.y);
		s.value4b(v.z);
		s.value4b(v.u);
		s.value4b(v.v);
		s.value4b(v.nx);
		s.value4b(v.ny);
		s.value4b(v.nz);
	});

	s.container4b(o.indices, 1 << 24);
}

tuple<mesh_ir, u64> load_obj(const string& path);
mesh_ir optimize_mesh(const string& path);
void save_mesh(mesh_ir& m);