#include "utils.h"
/**/
std::unordered_map<std::vector<int>, ap_int<INT_SIZE>, VectorHasher> PREVIOUS_VALUES;

ap_int<INT_SIZE> calc_num_of_arrangements_with_checks(std::vector<int> const& bip, int numberOfParts, int offset, bool isFirst) {
	if (!is_bip(bip)) {
		std::cout << "label appears only once";
		throw std::invalid_argument("label appears only once");
	}
	for (int i = 0; i < bip.size()/2; i++) {
		bool valueAppeared = false;
		for (int a : bip) {
			if (a == i) {
				valueAppeared = true;
				break;
			}
		}
		if (!valueAppeared) {
			std::cout << "a value does not appear";
			throw std::invalid_argument("a value does not appear");
		}
	}

	return calc_num_of_arrangements(bip, numberOfParts, offset, isFirst);
}

ap_int<INT_SIZE> calc_num_of_arrangements(std::vector<int> const& bip, int numberOfParts, int offset, bool isFirst) {
	int numberOfLines = bip.size() / 2;
	if (numberOfLines < 4)
		return calc_small_bip(bip, numberOfLines);


	ap_int<INT_SIZE> total = 0;
	std::vector<std::vector<int>> allLineIndeces = get_all_indeces(bip, numberOfLines);
	std::vector<std::vector<bool>> crossingMatrix = get_crossingMatrix(bip, allLineIndeces, numberOfLines);

	int splitLine = select_line(allLineIndeces, crossingMatrix, numberOfLines);

	PartialOrder partialOrder = get_partial_order(splitLine, allLineIndeces[splitLine], bip, crossingMatrix);
	std::vector<std::vector<int>> topSortings = partialOrder.all_top_sortings();


	std::vector<int> leftHalf;
	std::vector<int> rightHalf;
	std::tie(leftHalf, rightHalf) = get_half_bips(allLineIndeces[splitLine], bip, partialOrder.lineLabels);

	std::vector<std::vector<int>> fixedlLeftLineIndeces = get_all_indeces(leftHalf, (leftHalf.size() + partialOrder.lineLabels.size()) / 2);
	std::vector<std::vector<int>> fixedRightLineIndeces = get_all_indeces(rightHalf, (rightHalf.size() + partialOrder.lineLabels.size()) / 2);

	int printNumber = topSortings.size() /(10 * numberOfParts) + 1;
	int i;
	int limit = (topSortings.size() - offset) / numberOfParts;
	for (int k = 0; k < limit; k++) {
		i = numberOfParts * k + offset;
		if (isFirst && k % printNumber == 0)
			std::cout	<< "Sorting \t" << i + 1 << "/" << topSortings.size() 
						<< "\nValues Saved: \t" << PREVIOUS_VALUES.size() 
						<< "\nCurrent Total: \t" << total 
						<< "\n\n";


		std::vector<int> sorting = topSortings[i];
		std::vector<int> rightBip = rightHalf;
		std::vector<int> leftBip = leftHalf;

		leftBip.reserve(leftBip.size() + distance(sorting.begin(), sorting.end()));
		leftBip.insert(leftBip.end(), sorting.rbegin(), sorting.rend());

		rightBip.reserve(rightBip.size() + distance(sorting.begin(), sorting.end()));
		rightBip.insert(rightBip.end(), sorting.begin(), sorting.end());

		std::vector<std::vector<int>> leftLineIndeces = fixedlLeftLineIndeces;
		std::vector<std::vector<int>> rightLineIndeces = fixedRightLineIndeces;

		for(int j = 0; j < sorting.size(); j++){
			leftLineIndeces[sorting[j]].push_back(j + leftHalf.size());
			rightLineIndeces[sorting[j]].push_back(j + rightHalf.size());
		}

		make_canonical(leftBip, leftLineIndeces);
		make_canonical(rightBip, rightLineIndeces);

		ap_int<INT_SIZE> leftN;
		ap_int<INT_SIZE> rightN;

		if (PREVIOUS_VALUES.find(leftBip) != PREVIOUS_VALUES.end()) {
			leftN = PREVIOUS_VALUES[leftBip];
		}
		else {
			leftN = calc_num_of_arrangements(leftBip);
			PREVIOUS_VALUES[leftBip] = leftN;
		}
		if (PREVIOUS_VALUES.find(rightBip) != PREVIOUS_VALUES.end()) {
			rightN = PREVIOUS_VALUES[rightBip];
		}
		else {
			rightN = calc_num_of_arrangements(rightBip);
			PREVIOUS_VALUES[rightBip] = rightN;
		}

		total += leftN * rightN;
	}

	return total;
}


int calc_small_bip(std::vector<int>const& bip, int numberOfLines) {
	if (numberOfLines < 3) {
		return 1;
	}
	else if (numberOfLines == 3) {
		if (bip[0] == bip[3] && bip[1] == bip[4])
			return 2;

		else
			return 1;
	}
	else if (numberOfLines == 4) {
		int numberOfCrossings = 0;
		for (int a = 0; a < numberOfLines; a++) {
			for (int b = 0; b < numberOfLines; b++) {
				numberOfCrossings += intersect(a, b, bip);
			}
		}
		numberOfCrossings /= 2;

		if (numberOfCrossings == 6) {
			return 8;
		}
		else if (numberOfCrossings == 5) {
			return 3;
		}
		else if (numberOfCrossings == 3 || numberOfCrossings == 4) {
			int linesCrossed;
			int numberOfDeg3 = 0;
			for (int a = 0; a < numberOfLines; a++) {
				linesCrossed = 0;
				for (int b = 0; b < numberOfLines; b++) {
					linesCrossed += intersect(a, b, bip);
				}
				if (linesCrossed >= 2) {
					numberOfDeg3++;
				}
			}
			if (numberOfDeg3 == 3) {
				return 2;
			}
		}
		return 1;
	}
}

std::vector<std::vector<int>> get_all_indeces(std::vector<int>const& bip, int numberOfLines) {
	std::vector<std::vector<int>> allIndeces(numberOfLines, std::vector<int>());

	for (int i = 0; i < bip.size(); i++) {
		allIndeces[bip[i]].push_back(i);
	}

	return allIndeces;
}

std::vector<std::vector<bool>> get_crossingMatrix(std::vector<int>const& bip, std::vector<std::vector<int>>const& allLineIndeces, int numberOfLines) {
	std::vector<std::vector<bool>> crossingMatrix = std::vector<std::vector<bool>>(numberOfLines, std::vector<bool>(numberOfLines, 0));
	for (unsigned int i = 0; i < numberOfLines; i++) {
		for (unsigned int j = i+1; j < numberOfLines; j++) {
			crossingMatrix[i][j] = intersect(allLineIndeces[i], allLineIndeces[j]);
			crossingMatrix[j][i] = crossingMatrix[i][j];
		}
	}
	return crossingMatrix;
}

int select_line(std::vector<std::vector<int>>const& allLineIndeces, std::vector<std::vector<bool>>const& crossingMatrix, int numberOfLines){
	int size = 2*numberOfLines;
	int minScore	= size;
	int minScore2	= size;
	int minLabel = 0;
	int score;
	int score2;
	int degree;
	//bool foundALine = false;

	for (int a = 0; a < numberOfLines; a++) {
		degree = 0;
		for (bool b : crossingMatrix[a]) {
			degree += b;
		}

		std::vector<int> aIdx = allLineIndeces[a];

		//score = -std::min(aIdx[1] - aIdx[0] - 1, size - (aIdx[1] - aIdx[0]) - 1);
		//score += degree;
		score		= std::max(aIdx[1] - aIdx[0] - 1, size - (aIdx[1] - aIdx[0]) - 1) + degree;
		score2		= std::min(aIdx[1] - aIdx[0] - 1, size - (aIdx[1] - aIdx[0]) - 1) + degree;

		if (score < minScore || (score == minScore && score2 < minScore2)) {
			minScore = score;
			minScore2 = score2;
			minLabel = a;
			//foundALine = true;
		}
	}

	/*
	if (!foundALine) {
		std::cout << "no line selected";
		throw std::invalid_argument("no line selected");
	}*/

	return minLabel;
}

PartialOrder get_partial_order(int splitLabel, std::vector<int> splitIdx, std::vector<int>const& bip, std::vector<std::vector<bool>>const& crossingMatrix){
	int degree = 0;
	for (bool b : crossingMatrix[splitLabel])
		degree += b;
	
	PartialOrder partialOrder(degree);
	std::vector<int> crossingLines;

	for (int i = splitIdx[0] + 1; i < splitIdx[1]; i++) {
		if (crossingMatrix[splitLabel][bip[i]]) {
			for (int j = 0; j < crossingLines.size(); j++) {
				if (!crossingMatrix[bip[i]][crossingLines[j]]) {
					partialOrder.add_edge(crossingLines.size(), j); //j needs to come above the new element, i.e. AFTER so we need new > j
				}
			}
			crossingLines.push_back(bip[i]);
		}
	}

	partialOrder.lineLabels = crossingLines;

	return partialOrder;
}

std::tuple<std::vector<int>, std::vector<int>> get_half_bips(std::vector<int> splitIdx, std::vector<int>const& bip, std::vector<int>const& crossingLines){
	int size = bip.size();
	auto kItr1 = bip.begin() + splitIdx[0];
	auto kItr2 = bip.begin() + splitIdx[1];
	std::vector<int> rightHalf(kItr1+1, kItr2);
	std::vector<int> leftHalf(kItr2+1, bip.end());
	leftHalf.reserve(leftHalf.size() + std::distance(bip.begin(), kItr1));
	leftHalf.insert(leftHalf.end(), bip.begin(), kItr1);

	//renormalize: crossing lines should have labels 0,...,h in the order they appear in crossingLines 
	//other lines get other labels in arbitrary order

	std::unordered_map<int, int> crossLabels;
	std::unordered_map<int, int> newLabels;
	int currentLabel = 0;
	for (int a : crossingLines) {
		crossLabels[a] = currentLabel;
		currentLabel++;
	}

	newLabels = crossLabels;
	currentLabel = crossingLines.size();
	for (int& a : leftHalf) {
		if (newLabels.find(a) == newLabels.end()) {
			newLabels[a] = currentLabel;
			a = currentLabel;
			currentLabel++;
		}
		else {
			a = newLabels[a];
		}
	}

	newLabels = crossLabels;
	currentLabel = crossingLines.size();
	for (int& a : rightHalf) {
		if (newLabels.find(a) == newLabels.end()) {
			newLabels[a] = currentLabel;
			a = currentLabel;
			currentLabel++;
		}
		else {
			a = newLabels[a];
		}
	}

	return std::make_tuple(leftHalf, rightHalf);
}

void make_canonical(std::vector<int>& bip, std::vector<std::vector<int>>const& lineIndeces){
	int size = bip.size();
	int numberOfLines = size / 2;
	std::vector<std::vector<int>> promisingIndeces;

	//get promising indeces
	int minScore = size;
	for (std::vector<int>const& x : lineIndeces) {
		int score = std::min(x[1] - x[0], size - (x[1] - x[0]));
		if (score < minScore) {
			promisingIndeces.clear();
			promisingIndeces.push_back(x);
			minScore = score;
		}
		else if (score == minScore) {
			promisingIndeces.push_back(x);
		}
	}

	//get minimal bip
	std::vector<int> bestBip = bip;

	for (std::vector<int>& x : promisingIndeces) {
		std::unordered_map<int, int> newLabels;
		newLabels.reserve(numberOfLines);
		std::vector<int> currentBip;
		currentBip.reserve(size);
		bool isSmaller = false;
		int label = 0;

		for (int i = 0; i < size; i++) {
			int a = bip[(i + x[0]) % size];
			if (newLabels.find(a) == newLabels.end()) {
				newLabels[a] = label;
				label++;
			}

			if (isSmaller || newLabels[a] == bestBip[i]) {
				currentBip.push_back(newLabels[a]);
			}
			else if (newLabels[a] < bestBip[i]) {
				currentBip.push_back(newLabels[a]);
				isSmaller = true;
			}
			else {
				break;
			}
		}
		if (currentBip.size() == size)
			bestBip = currentBip;

	}

	for (std::vector<int>& x : promisingIndeces) {
		std::unordered_map<int, int> newLabels;
		newLabels.reserve(numberOfLines);
		std::vector<int> currentBip;
		currentBip.reserve(size);
		bool isSmaller = false;
		int label = 0;

		for (int i = 0; i < size; i++) {
			int a = bip.rbegin()[(i + size - x[1] - 1) % size];
			if (newLabels.find(a) == newLabels.end()) {
				newLabels[a] = label;
				label++;
			}

			if (isSmaller || newLabels[a] == bestBip[i]) {
				currentBip.push_back(newLabels[a]);
			}
			else if (newLabels[a] < bestBip[i]) {
				currentBip.push_back(newLabels[a]);
				isSmaller = true;
			}
			else {
				break;
			}
		}
		if (currentBip.size() == size)
			bestBip = currentBip;
	}

	bip = bestBip;
}



bool intersect(int a, int b, std::vector<int>const& bip) {
	if (a == b) {
		return false;
	}
	int state = -1;

	for (int c : bip) {
		if (c == a) {
			if (state == a)
				return false;
			else
				state = a;
		}
		else if (c == b) {
			if (state == b)
				return false;
			else
				state = b;
		}
	}
	return true;
}

bool intersect(std::vector<int> aIdx, std::vector<int> bIdx) {
	bool u = aIdx[0] < bIdx[0];
	bool v = bIdx[0] < aIdx[1];
	bool w = aIdx[1] < bIdx[1];

	bool x = bIdx[0] < aIdx[0];
	bool y = aIdx[0] < bIdx[1];
	bool z = bIdx[1] < aIdx[1];
	
	return (u && v && w) || (x && y && z);
}

std::tuple<int, int> get_indeces(int a, std::vector<int>const& bip) {
	int idx1 = -1;
	int idx2 = -1;

	for (int i = 0; i < bip.size(); i++) {
		if (bip[i] == a) {
			if (idx1 == -1) {
				idx1 = i;
			}
			else {
				idx2 = i;
				break;
			}
		}
	}
	return std::make_tuple(idx1, idx2);
}

bool is_bip(std::vector<int>const& bip) {
	for (int a : bip) {
		int number = 0;
		for (int b : bip) {
			if (a == b) {
				number++;
			}
		}
		if (number != 2) {
			return false;
		}
	}
	return true;
}


