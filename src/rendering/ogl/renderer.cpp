#include "renderer.h"

#include "src/rendering/vertex.h"

renderer::renderer(window& w) :
	win(w),
	model(buffer_access_type::StaticDraw, buffer_type::VertexData),
	vert()
{
	vertex v[3] = {{{0.0f, 0.5f, 0.0f}}, {{-0.3f, -0.5f, 0.0f}}, {{0.3f, -0.5f, 0.0f}}};
	model.bind();
	model.setData(v, sizeof(vertex) * 3, 0);
	
	vertex::prepareVAO(vert);
}

void renderer::render()
{
	
}
