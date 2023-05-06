#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>

namespace ag
{
	template <typename T>
	struct Node
	{
		glm::vec3 position;
		T data;

		Node(glm::vec3 pos, T val)
		{
			position = pos;
			data = val;
		}
	};

	template <typename T, typename IDType>
	class Grid
	{
		Grid(size_t x, size_t y, float size)
		{
			x_size = x;
			y_size = y;

			cellSize = size;

			grid.resize(x_size);
			for (size_t i = 0; i < x_size; i++)
			{
				grid[i].resize(y_size);
			}
		}

		/**
		* Gets all elements in adjacent cells.
		*/
		std::vector<T> Query(glm::vec3 pos)
		{
			size_t x = pos.x / cellSize;
			size_t y = pos.y / cellSize;

			std::vector<T> out;

			for (size_t i = x - 1; i <= x + 1; i++)
			{
				for (size_t j = y - 1; j <= y + 1; j++)
				{
					for (size_t k = 0; k < grid[i][j]; k++)
					{
						out.push_back(nodes[grid[i][j][k].data);
					}
				}
			}

			return out;
		}

		/**
		* Adds an object to the grid
		*/
		void Insert(IDType id, T val, glm::vec3 pos)
		{
			nodes.insert({ id, Node(pos, val) });

			size_t x = pos.x / cellSize;
			size_t y = pos.y / cellSize;

			grid[x][y].push_back(id);
		}

		/**
		* Adjusts an object's position in the grid
		*/
		void Move(IDType id, glm::vec3 newPos)
		{
			size_t newX = newPos.x / cellSize;
			size_t newY = newPos.y / cellSize;

			auto f = nodes.find(id);
			if (f == nodes.end())
			{
				/// TODO: throw err
				return;
			}

			size_t oldX = f->pos.X / cellSize;
			size_t oldY = f->pos.Y / cellSize;
			// Node moved to new cell
			if (newX != oldX || newY != oldY)
			{
				grid[oldX][oldY].erase(grid[oldX][oldY].find(id));
				grid[newX][newY].push_back(id);
			}

			f->pos = newPos;
		}

	private:
		
		std::vector<std::vector<std::vector<IDType>>> grid;
		std::unordered_map<IDType, Node> nodes;

		size_t x_size;
		size_t y_size;
		float cellSize;
	};
}