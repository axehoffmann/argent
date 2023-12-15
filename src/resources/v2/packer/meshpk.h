#pragma once

#include "lib/string.h"
#include "lib/tuple.h"
#include "lib/vector.h"

#include "rendering/vertex.h"

#include "meshoptimizer.h"

struct mesh_ir
{
	vector<vertex> vertices;
	vector<u32> indices;

	u64 tris;
};

mesh_ir load_mesh(const string& path);
void pack_mesh(const string& path);
