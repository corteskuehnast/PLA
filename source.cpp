#include <vector>
#include <chrono>

#include "ap/ap.hpp"

#include "PartialOrder.h"
#include "utils.h"
#include "bipermutations.h"



int main(int argc, char* argv[]) {
    int numberOfParts = 1;
    int offset = 0;

    if (argc == 3) {
        numberOfParts = atoi(argv[1]);
        offset = atoi(argv[2]);
        offset = offset % numberOfParts;
    }
    else if (argc != 1) {
        std::cout << "invalid number of arguments" << std::endl;
        return -1;
    }

    auto begin = std::chrono::high_resolution_clock::now();

    ap_int<INT_SIZE> N;
    N = calc_num_of_arrangements_with_checks(get_square_bip(3,7), numberOfParts, offset, true);

    std::cout << "Result: \t\t\t" << N << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time in seconds:\t\t";
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000 << std::endl;

	return 0;
}