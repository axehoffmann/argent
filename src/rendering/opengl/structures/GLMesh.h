#pragma once

#include "rendering/opengl/GL.h"
#include "rendering/opengl/structures/GLBuffer.h"
#include "rendering/opengl/structures/GLVAO.h"

#include "resources/Mesh.h"
#include "resources/AssetManager.h"

#include "debug/log/Log.h"

namespace ag
{
	struct GLMesh
	{
		ag::GLBuffer vbo;
		// ag::GLBuffer ebo;
		ag::GLVAO vao;

		size_t indexCount;

		static GLMesh FromData(std::vector<glm::vec3> positions)
		{
			GLMesh mesh = GLMesh(BufferAccessType::StaticDraw);

			mesh.vao.Bind();
			mesh.vbo.SetData(positions);
			mesh.vao.InitialiseAttribute(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);

			return mesh;
		}

		static GLMesh FromResource(uint32_t resourceID)
		{
			std::weak_ptr<ag::Mesh> wresource = ag::AssetManager::Fetch<ag::Mesh>(resourceID);
			if (auto resource = wresource.lock())
			{
				GLMesh mesh = GLMesh(BufferAccessType::StaticDraw);

				mesh.vao.Bind();
				mesh.vbo.SetData(resource->vertices);
				mesh.vao.InitialiseAttribute(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);

				return mesh;
			}

			Log::Error(sfmt("Couldn't get Mesh resource with ID {} when trying to build GLMesh", resourceID));
			throw std::runtime_error("Couldn't get Mesh resource");
		}

		void Bind()
		{
			vao.Bind();
		}

	private:
		GLMesh(GLMesh&& other) : vbo(std::move(other.vbo)), vao(std::move(other.vao)) {}
		GLMesh(BufferAccessType access) : vao(), vbo(BufferType::VertexData, access) {}

	};
}