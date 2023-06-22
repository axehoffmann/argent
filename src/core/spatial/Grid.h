#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>
#include <algorithm>
namespace ag
{
	template <typename T>
	struct Node
	{
		glm::vec3 pos;
		T data;

		Node()
		{
			pos = glm::vec3();
			data = T();
		}
		Node(glm::vec3 position, T val)
		{
			pos = position;
			data = val;
		}
	};

	/**
	* A 2D static spatial index.
	* @tparam T The type of data to store.
	* @tparam IDType The type used to index and access objects.
	*/
	template <typename T, typename IDType>
	class Grid
	{
	public:
		/**
		* @param x The amount of cells along the x axis.
		* @param y The amount of cells along the y axis.
		* @param size The raw size each cell covers.
		*/
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
		* @param pos The location of the query. z coordinate is ignored.
		*/
		std::vector<T> Query(glm::vec3 pos)
		{
			size_t x = pos.x / cellSize;
			size_t y = pos.y / cellSize;

			std::vector<T> out;

			for (size_t i = x - 1; i <= x + 1; i++)
			{
				if (i < 0 || i >= grid.size())
					continue;

				for (size_t j = y - 1; j <= y + 1; j++)
				{
					if (j < 0 || j >= grid[0].size())
						continue;

					for (size_t k = 0; k < grid[i][j].size(); k++)
					{
						out.push_back(nodes[grid[i][j][k]].data);
					}
				}
			}

			return out;
		}

		/**
		* Adds an object to the grid.
		* @param id The id of the inserted object.
		* @param val The object data.
		* @param pos The position of the object.
		*/
		void Insert(IDType id, T val, glm::vec3 pos)
		{
			nodes.insert({ id, Node(pos, val) });

			size_t x = pos.x / cellSize;
			size_t y = pos.y / cellSize;

			grid[x][y].push_back(id);
		}

		/**
		* Adjusts an object's position in the grid.
		* @param id The id of the object to move.
		* @param newPos The new position of the object
		*/
		void Move(IDType id, glm::vec3 newPos)
		{
			size_t newX = newPos.x / cellSize;
			size_t newY = newPos.y / cellSize;

			auto it = nodes.find(id);
			if (it == nodes.end())
			{
				/// TODO: throw err
				return;
			}

			Node<T> f = it->second;
			size_t oldX = f.pos.x / cellSize;
			size_t oldY = f.pos.y / cellSize;
			// Node moved to new cell
			if (newX != oldX || newY != oldY)
			{
				std::vector<IDType>& target = grid[oldX][oldY];
				target.erase(std::find(target.begin(), target.end(), id));
				grid[newX][newY].push_back(id);
			}

			f.pos = newPos;
		}

	private:
		
		std::vector<std::vector<std::vector<IDType>>> grid;
		std::unordered_map<IDType, Node<T>> nodes;

		size_t x_size;
		size_t y_size;
		float cellSize;
	};
}