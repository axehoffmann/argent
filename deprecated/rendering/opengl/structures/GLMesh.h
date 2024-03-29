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

		/**
		* Generates a GLMesh from a set of vertices
		* @param positions The set of vertices
		*/
		static GLMesh FromData(std::vector<glm::vec3> positions)
		{
			GLMesh mesh = GLMesh(BufferAccessType::StaticDraw);

			mesh.vao.Bind();
			mesh.vbo.SetData(positions);
			mesh.InitialiseAttributes();
			mesh.indexCount = positions.size();

			return mesh;
		}

		/**
		* Generates a GLMest from a Mesh resource
		* @param resourceID The ID of the Mesh resource
		*/
		static GLMesh FromResource(uint32_t resourceID)
		{
			std::weak_ptr<ag::Mesh> wresource = ag::AssetManager::Fetch<ag::Mesh>(resourceID);
			if (auto resource = wresource.lock())
			{
				GLMesh mesh = GLMesh(BufferAccessType::StaticDraw);

				mesh.vao.Bind();
				mesh.vbo.SetData(resource->vertices);
				mesh.InitialiseAttributes();
				mesh.indexCount = resource->vertices.size();

				return mesh;
			}

			Log::Error(sfmt("Couldn't get Mesh resource with ID {} when trying to build GLMesh", resourceID));
			throw std::runtime_error("Couldn't get Mesh resource");
		}

		/**
		* Binds the mesh, using it in the next draw call
		*/
		void Bind()
		{
			vao.Bind();
		}
		
		/**
		* Initialises the attributes of the mesh's VAO
		*/
		void InitialiseAttributes()
		{
			vao.InitialiseAttribute(0, 3, GL_FLOAT, false, 14 * sizeof(float), 0);
			vao.InitialiseAttribute(1, 2, GL_FLOAT, false, 14 * sizeof(float), 3 * sizeof(float));
			vao.InitialiseAttribute(2, 3, GL_FLOAT, false, 14 * sizeof(float), 5 * sizeof(float));
			vao.InitialiseAttribute(3, 3, GL_FLOAT, false, 14 * sizeof(float), 8 * sizeof(float));
			vao.InitialiseAttribute(4, 3, GL_FLOAT, false, 14 * sizeof(float), 11 * sizeof(float));
		}

	private:
		GLMesh(GLMesh&& other) : vbo(std::move(other.vbo)), vao(std::move(other.vao)) {}
		GLMesh(BufferAccessType access) : vao(), vbo(BufferType::VertexData, access) {}

	};
}