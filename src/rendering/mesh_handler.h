#pragma once

#include "resources/Mesh.h"
#include "lib/basic.h"
#include "lib/vector.h"

/// TODO: Figure out less dependent communication regarding this mesh metadata
struct mesh_handler
{
	vector<glm::vec4> baseBoundingSpheres;
};