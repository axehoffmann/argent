#pragma once
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace agt
{
	template <typename value>
	struct grid
	{
		grid(int x, int y, float cell_size) : width(x), height(y), cellsize(cell_size)
		{
			data = new std::vector<node>*[x];
			for (size_t i = 0; i < x; i++)
			{
				data[i] = new std::vector<node>[y];
			}
		}

		~grid()
		{
			for (size_t i = 0; i < width; i++)
			{
				delete[] data[i];
			}
			delete[] data;
		}

		std::vector<value> query_box(glm::vec2 p1, glm::vec2 p2)
		{

		}

		std::vector<value> query_circle(glm::vec2 p, float r)
		{

		}

	private:
		struct node
		{
			glm::vec2 pos;
			value val;
		};

		std::vector<node>** data;
		int width;
		int height;
		float cellsize;

		bool in_bounds(glm::vec2 pos)
		{
			return (pos.x < 0 || pos.x > width ||
				pos.y < 0 || pos.y > height)
		}

		glm::ivec2 to_grid_coords(glm::vec2 pos)
		{
			return { (int)(pos.x / cellsize), (int)(pos.y / cellsize) };
		}
	};
}