#pragma once

#include "basic.h"
#include "bitmap.h"
#include "ptr.h"
#include "queue.h"
#include "sync.h"
#include "vector.h"

template <typename T>
class job_graph
{
public:
	 
	job_graph(vector<node>&& nds)
		: nodes(nds),
		  completed(0)
	{
		for (u16 i = 0; node& n : nodes)
		{
			if (n.dependencies == 0)
				start_nodes.push_back(i);

			n.waiting_on = n.dependencies;

			i++;
		}
	}

	/**
	 * Repeatedly consumes tasks from the graph until all tasks are completed.
	*/
	template <typename Func>
	void consume(Func f)
	{
		while (completed != nodes.size())
		{
			u16 n;

			// Try grab a node from the queue
			{
				lock_guard<mutex> lock(queue_mutex);
				if (job_queue.empty()) continue;

				n = job_queue.pop();
			}

			f(nodes[n].object.get_temp());

			// Advance the graph
			completed.fetch_add(1);
			for (u16 ch : nodes[n].children)
			{
				if (ch.waiting_on.fetch_add(-1) == 1)
				{
					lock_guard<mutex> lock(queue_mutex);
					job_queue.push(ch.id);
				}
			}
		}
	}

	/**
	 * Resets the job graph to its initial state. Assumes that no threads are currently consuming the graph.
	*/
	void reset()
	{
		for (node& n : nodes)
		{
			n.waiting_on = n.dependencies;
		}

		job_queue.clear();

		for (u16 s : start_nodes)
		{
			job_queue.push(s);
		}
	}
	
	struct node
	{
		node(ptr<T>&& data, vector<u16>&& ch, u16 deps, u16 idx)
			: object(data), id(idx), children(ch), dependencies(deps), waiting_on(0) {}

		node(node&& other)
			: object(other.object), id(other.id), children(other.children), dependencies(other.dependencies), waiting_on(other.waiting_on) {}
		node& operator=(node&& other)
		{
			object = std::move(other.object);
			id = other.id;
			children = std::move(other.children);
			dependencies = other.dependencies;
			waiting_on = other.waiting_on;
		}

		ptr<T> object;
		u16 id;
		vector<u16> children;
		u16 dependencies;
		atomic<u16> waiting_on;
	};

private:

	mutex queue_mutex;
	queue<u16> job_queue; // queue of nodes ready to be consumed

	atomic<u16> completed; // total count of consumed nodes

	vector<node> nodes;
	vector<u16> start_nodes; // nodes that can be consumed immediately (no dependencies)
};