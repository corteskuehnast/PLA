#pragma once
#include <vector>

class PartialOrder{
public:
	PartialOrder(int V);
	void add_edge(int a, int b);
	std::vector<std::vector<int>> all_linear_extensions();

	std::vector<int> lineLabels;

private:
	int V;
	//std::vector<std::vector<bool>>	A;
	std::vector<std::vector<int>>	adj;
	std::vector<int>		indeg;
	std::vector<std::vector<int>>	results;

	void all_top_sortings_recursion(std::vector<int>& res, std::vector<bool>& visited);
};

