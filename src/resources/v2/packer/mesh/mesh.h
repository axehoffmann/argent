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
	f32 tx, ty, tz, tw;
};

struct mesh_ir
{
	glm::vec4 boundingSphere;
	vector<basic_vertex> vertices;
	vector<u32> indices;
};

template <typename S>
void serialize(S& s, mesh_ir& o)
{
	s.object(o.boundingSphere, [](S& s, glm::vec4& v)
	{
		s.value4b(v.x);
		s.value4b(v.y);
		s.value4b(v.z);
		s.value4b(v.w);
	});
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
		s.value4b(v.tx);
		s.value4b(v.ty);
		s.value4b(v.tz);
	});

	s.container4b(o.indices, 1 << 24);
}

void compile_mesh(const string& path);
mesh_ir load_obj(const string& path);
mesh_ir optimize_mesh(mesh_ir&& m);
void save_mesh(mesh_ir& m, const string& path);
void calculate_bounds(mesh_ir& m);

mesh_ir load_mesh(const string& path);
