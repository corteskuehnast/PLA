#include "PartialOrder.h"

PartialOrder::PartialOrder(int V): V(V) {
	adj = std::vector<std::vector<int>>(V, std::vector<int>());
	indeg = std::vector<int>(V, 0);
}

void PartialOrder::add_edge(int a, int b){
	adj[a].push_back(b);
	indeg[b]++;
}

std::vector<std::vector<int>> PartialOrder::all_top_sortings(){
	std::vector<bool> visited(V, false);
	std::vector<int> res;

	all_top_sortings_recursion(res, visited);

	return results;
}

void PartialOrder::all_top_sortings_recursion(std::vector<int>& res, std::vector<bool>& visited){
	bool newVertexViseted = false;

	for (int i = 0; i < V; i++) {
		if (indeg[i] == 0 && !visited[i]) {
			for (int neighbor : adj[i]) {
				indeg[neighbor]--;
			}

			res.push_back(i);
			visited[i] = true;

			all_top_sortings_recursion(res, visited);

			visited[i] = false;
			res.pop_back();
			for (int neighbor : adj[i]) {
				indeg[neighbor]++;
			}

			newVertexViseted = true;
		}
	}

	if (!newVertexViseted) {
		results.push_back(res);
	}
}
