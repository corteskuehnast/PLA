#pragma once
#include <vector>

//bipermutations of kxl rectangles in R_4
const std::vector<int> rectBip2x2 = { 0,2,1,4,3,5,6,1,3,0,5,2,4,6 };
const std::vector<int> rectBip2x3 = { 0,2,1,4,3,5,7,6,1,8,0,3,6,8,5,2,4,7 };
const std::vector<int> rectBip3x4 = { 0,3,1,10,2,4,6,10,7,5,8,11,9,2,12,1,5,0,9,4,8,12,7,3,6,11 };

const std::vector<int> rectBip4x4 = { 0,4,1,12,2,5,3,8,6,9,12,10,7,11,13,3,7,2,14,1,6,0,11,5,10,14,9,4,8,13 };
const std::vector<int> rectBip4x5 = { 0,4,1,13,2,5,3,8,6,9,13,10,7,11,14,12,3,15,2,7,1,16,0,6,12,16,11,5,10,15,9,4,8,14 };
const std::vector<int> rectBip4x7 = { 0,4,1,16,2,5,3,9,6,10,16,11,7,12,17,13,8,14,18,15,3,19,2,8,1,20,0,7,15,20,14,6,13,19,12,5,11,18,10,4,9,17 };
const std::vector<int> rectBip4x9 = { 0,4,1,19,2,5,3,10,6,11,19,12,7,13,20,14,8,15,21,16,9,17,22,18,3,23,2,9,1,24,0,8,18,24,17,7,16,23,15,6,14,22,13,5,12,21,11,4,10,20 };
const std::vector<int> rectBip5x6 = { 0,5,1,16,2,6,20,3,4,7,10,20,11,8,12,16,13,9,14,17,15,4,18,3,9,2,19,1,8,0,15,7,14,19,13,6,12,18,11,5,10,17 };

const std::vector<int> rectBip6x6 = { 0,6,1,19,2,7,3,18,4,8,5,
                                    12,9,13,18,14,10,15,19,16,11,17,20,
                                    5,11,4,21,3,10,2,22,1,9,0,
                                    17,8,16,22,15,7,14,21,13,6,12,20 };

const std::vector<int> rectBip6x7 = { 0,6,1,20,2,7,3,19,4,8,5,
									12,9,13,19,14,10,15,20,16,11,17,21,18,
									5,22,4,11,3,23,2,10,1,24,0,
									9,18,24,17,8,16,23,15,7,14,22,13,6,12,21 };

const std::vector<int> rectBip7x8 = { 0,7,1,24,2,8,3,23,4,9,5,22,6,
                                    10,14,22,15,11,16,23,17,12,18,24,19,13,20,25,21,
                                    6,26,5,13,4,27,3,12,2,28,1,11,0,
                                    21,10,20,28,19,9,18,27,17,8,16,26,15,7,14,25 };

const std::vector<int> rectBip8x8 = { 0,8,1,26,2,9,3,25,4,10,5,24,6,11,7,
                                    16,12,17,24,18,13,19,25,20,14,21,26,22,15,23,27,
                                    7,15,6,28,5,14,4,29,3,13,2,30,1,12,0,
                                    23,11,22,30,21,10,20,29,19,9,18,28,17,8,16,27 };

const std::vector<int> rectBip8x9 = { 0,8,1,27,2,9,3,26,4,10,5,25,6,11,7,
                                    16,12,17,25,18,13,19,26,20,14,21,27,22,15,23,28,24,
                                    7,29,6,15,5,30,4,14,3,31,2,13,1,32,0,
                                    12,24,32,23,11,22,31,21,10,20,30,19,9,18,29,17,8,16,28 };





std::vector<int> get_square_bip(int region, int size) {
	/**
	* calculates the boundary bipermutation of a square that is aligned with the axes
	*/
	std::vector<int> bip;
	if (region == 3) {
		for (int i = 0; i < size; i++) {
			bip.push_back(i);
			bip.push_back(4 * size - 2 - i);
		}
		bip.pop_back();

		for (int i = 0; i < size; i++) {
			bip.push_back(3 * size - 1 - i);
			bip.push_back(size + i);
		}

		for (int i = 0; i < size; i++) {
			bip.push_back(size - 1 - i);
			bip.push_back(2 * size + i);
		}
		bip.pop_back();

		for (int i = 0; i < size; i++) {
			bip.push_back(3 * size - 1 + i);
			bip.push_back(2 * size - 1 - i);
		}

		return bip;
	}
	else if (region == 4) {
		for (int i = 0; i < size; i++) {
			bip.push_back(5 * size - 2 - i);
			bip.push_back(i);
			bip.push_back(4 * size - 2 - i);
		}
		bip.pop_back();

		for (int i = 0; i < size; i++) {
			bip.push_back(3 * size - 1 - i);
			bip.push_back(size + i);
			bip.push_back(4 * size - 1 + i);
		}
		//bip.pop_back();

		for (int i = 0; i < size; i++) {
			//bip.push_back(5 * size - 2 + i);
			bip.push_back(size - 1 - i);
			bip.push_back(5 * size - 2 + 1 + i); //
			bip.push_back(2 * size + i);
		}
		//bip.pop_back();

		for (int i = 0; i < size; i++) {
			//bip.push_back(3 * size - 1 + i);
			//bip.push_back(2 * size - 1 - i);
			//bip.push_back(6 * size - 3 - i);
			bip.push_back(6 * size - 2 - i);
			bip.push_back(2 * size - 1 - i);
			bip.push_back(3 * size - 1 + 1 + i);
		}
		bip.pop_back();

		return bip;
	}

	std::cout << "region is not 3 or 4";
	throw std::invalid_argument("region is not 3 or 4");
}


