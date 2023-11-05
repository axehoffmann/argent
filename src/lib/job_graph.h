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

	/**
	 * Repeatedly consumes tasks from the graph until all tasks are completed.
	*/
	template <typename Func>
	void consume(Func f)
	{
		while (completed != nodes.size())
		{
			u32 node;

			// Try grab a node from the queue
			{
				lock_guard<mutex> lock(queue_mutex);
				if (job_queue.empty()) continue;

				node = job_queue.pop();
			}

			f(nodes[node].object);

			// Advance the graph
			completed.fetch_add(1);
			for (u32 ch : nodes[node].children)
			{
				if (ch.waiting_on.fetch_add(-1) == 1)
				{
					lock_guard<mutex> lock(queue_mutex);
					job_queue.push(ch.id);
				}
			}
		}
	}
	

private:

	struct job_node
	{
		T* object;
		u32 id;
		vector<u32> children;
		vector<u32> dependencies;
		atomic<u16> waiting_on;
	};

	mutex queue_mutex;
	queue<u32> job_queue; // queue of nodes ready to be consumed

	atomic<u32> completed; // total count of consumed nodes

	vector<job_node> nodes;
	vector<u32> start_nodes; // nodes that can be consumed immediately (no dependencies)
};