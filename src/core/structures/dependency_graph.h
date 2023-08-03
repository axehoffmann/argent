#include <vector>

namespace agt
{
	template <typename T>
	class dependency_graph
	{
		// Acyclic graph
		struct node_t
		{
			T data;
			bool complete;
			size_t waiting_for_count;
			std::vector<size_t> next;
		};

		std::vector<size_t> beginning;
		std::vector<size_t> fringe;

		std::vector<node_t> nodes;

		void complete_step(size_t node)
		{
			if (node < 0 || node >= nodes.size())
				return;
			if (nodes[node].complete == true)
				return;

			nodes[node].complete = true;
			fringe.erase(std::find(nodes.begin(), nodes.end(), node));
			for (size_t i = 0; i < nodes[node].next.size(), i++)
			{
				node_t& n = nodes[node].next[i];
				n.waiting_for_count--;
				if (n.waiting_for_count == 0)
					fringe.push_back(i);
			}
		}

		void next_step()
		{
			return fringe.back();
		}

		void reset()
		{
			for (node_t& n : nodes)
			{
				n.complete = false;
				n.waiting_for_count = n.predecessors.size();
			}

			fringe = beginning;
		}
	};
}