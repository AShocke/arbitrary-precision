#include "../blocks.h"
#include <iostream>
#include <string>
#include <vector>
int main(int argc, char** argv) {
    if (argc < 4) {
	std::cout << "Give precision( negative, positive) then the number as a string";
    }
    else  {
	unsigned int negative = atoi(argv[1]);
	unsigned int positive = atoi(argv[2]);
	std::cout << negative << " " << positive << "\n";
	std::string num = argv[3];
	precision p = {negative, positive};
	blocks b = blocks(p, num);
	if (!b.is_fail_block()) {
	    std::vector<int> b_v = b.get_blocks();
	    precision get = b.get_precision();
	    std::cout << get.negative_precision << " " << get.positive_precision << "\n";
	    char c = '5';
	    std::cout << c - '0' << "\n";
	    for (unsigned i = 0; i < b_v.size();i++) {
		std::cout << b_v[i];
	    }
	    std::cout << "\n";
	    std::cout << b.to_string() << "\n";
	}
    }
}
