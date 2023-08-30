#pragma once
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <algorithm>
#include <iterator>

#include <functional>

namespace agt
{
	template <typename value>
	struct grid
	{
		using predicate_ptr = bool(*)(value, glm::vec2);

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

		std::vector<value> query_box(glm::vec2 p1, glm::vec2 p2, predicate_ptr predicate = static_cast<predicate_ptr>(default_predicate))
		{
			glm::ivec2 p1_grid = to_grid_coords(p1);
			glm::ivec2 p2_grid = to_grid_coords(p2);
			if (p1_grid.x > p2_grid.x) std::swap(p1_grid.x, p2_grid.x);
			if (p1_grid.y > p2_grid.y) std::swap(p1_grid.y, p2_grid.y);

			return query_box_filtered(p1_grid, p2_grid, predicate);
		}

		std::vector<value> query_circle(glm::vec2 p, float r, predicate_ptr predicate = static_cast<predicate_ptr>(default_predicate))
		{
			glm::ivec2 p1_grid = to_grid_coords({ p.x - r, p.y - r });
			glm::ivec2 p2_grid = to_grid_coords({ p.x + r, p.y + r });

			return query_box_filtered(p1_grid, p2_grid, [=](value v, glm::vec2 pos) { return (glm::length(pos - p) <= r) && predicate(v, pos); });
		}

		void insert(value val, glm::vec2 pos)
		{
			glm::ivec2 pos = to_grid_coords(pos);
			data[pos.x][pos.y].push_back(node{ pos, val }); 
		}

	private:
		bool default_predicate(value val, glm::vec2 pos)
		{
			return true;
		}

		struct node
		{
			glm::vec2 pos;
			value val;
		};

		std::vector<value> query_box_filtered(glm::ivec2 p1_grid, glm::ivec2 p2_grid, std::function<bool(value, glm::vec2)> predicate)
		{
			std::vector<value> out;
			for (size_t x = p1_grid.x; x < p2_grid.x; x++)
			{
				for (size_t y = p1_grid.y; y < p2_grid.y; y++)
				{
					/// TODO: can this be simplified?
					for (size_t i = 0; i < data[x][y].size(); i++)
					{
						if (predicate(data[x][y][i]))
							out.push_back(data[x][y][i].val);
					}
				}
			}
			return out;
		}

		std::vector<node>** data;
		int width;
		int height;
		float cellsize;

		bool in_bounds(glm::vec2 pos)
		{
			return (pos.x < 0 || pos.x > width ||
				pos.y < 0 || pos.y > height);
		}

		glm::ivec2 to_grid_coords(glm::vec2 pos)
		{
			glm::ivec2 out = { (int)(pos.x / cellsize), (int)(pos.y / cellsize) };
			out.x = out.x < 0 ? 0 : (out.x > width ? width : out.x);
			out.y = out.y < 0 ? 0 : (out.y > height ? height : out.y);
			return out;
		}
	};
}