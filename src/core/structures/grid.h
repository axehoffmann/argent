#pragma once
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace agt
{
	template <typename value>
	struct grid
	{
		grid(int x, int y) : width(x), height(y)
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


	private:
		struct node
		{
			glm::vec2 pos;
			value val;
		};

		std::vector<node>** data;
		int width;
		int height;
	};
}