#pragma once

#include "rendering/ogl/opengl.h"

#include "lib/functor.h"
#include "lib/vector.h"

class gltimer
{
public:
	gltimer(function<f32> cb);
	~gltimer();

	void start();
	void stop();

private:
	function<f32> callback;

	vector<f32> values;

	glhandle id;
};