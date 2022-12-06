#pragma once
#include <vector>
#include <tuple>
#include <algorithm>
#include <stdexcept>
#include <future>
#include <unordered_map>
#include <cmath>

#include "ap/ap.hpp"

#include "PartialOrder.h"

constexpr int INT_SIZE = 256;


struct VectorHasher {
	//from https://stackoverflow.com/questions/10405030/c-unordered-map-fail-when-used-with-a-vector-as-key
    int operator()(const std::vector<int>& V) const {
		int hash = V.size();
		for (auto& i : V) {
			hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
		}
		return hash;
    };
};
extern std::unordered_map<std::vector<int>, ap_int<INT_SIZE>, VectorHasher> PREVIOUS_VALUES;


ap_int<INT_SIZE> calc_num_of_arrangements(std::vector<int> const& bip, int numberOfParts = 1, int offset = 0, bool isFirst = false);
ap_int<INT_SIZE> calc_num_of_arrangements_with_checks(std::vector<int> const& bip, int numberOfParts = 1, int offset = 0, bool isFirst = true);


//the base case
int calc_small_bip(std::vector<int>const& bip, int numberOfLines);

//a vector of pairs of indeces, with bip[indeces[i][0]] = bip[indeces[i][1]] = i
std::vector<std::vector<int>> get_all_indeces(std::vector<int>const& bip, int numberOfLines);

//adjacency matirx of the intersection graph of the pseudolines
std::vector<std::vector<bool>> get_crossingMatrix(std::vector<int>const& bip, std::vector<std::vector<int>>const& allLineIndeces, int numberOfLines);

//the best line to split the problem with
int select_line(std::vector<std::vector<int>>const& allLineIndeces, std::vector<std::vector<bool>>const& crossingMatrix, int numberOfLines);

//partial order on the set of pseudolines crossing splitLine. used to get the linear extensions
PartialOrder get_partial_order(int splitLabel, std::vector<int> splitIdx, std::vector<int>const& bip, std::vector<std::vector<bool>>const& crossingMatrix);

//constructs the partial pibermutations coresponding to the two halves of the original problem. 
//not a bipermutation, because the crossing lines are missing
std::tuple<std::vector<int>, std::vector<int>> get_half_bips(std::vector<int> splitIdx, std::vector<int>const& bip, std::vector<int>const& crossingLines);

//transforms bip into its canonical representative
void make_canonical(std::vector<int>& bip, std::vector<std::vector<int>>const& lineIndeces);



bool intersect(int a, int b, std::vector<int>const& bip);
bool intersect(std::vector<int> aIdx, std::vector<int> bIdx);
std::tuple<int, int> get_indeces(int a, std::vector<int>const& bip);
bool is_bip(std::vector<int>const& bip);





