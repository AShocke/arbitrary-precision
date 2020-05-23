#include "../workspace.h"
#include <iostream>
#include <random>
#include <cmath>
#include <string.h>

void default_test() {
    int num1[100] = {0};
    int num2[100] = {0};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 9);
    for (int i = 0; i < 99; i++) {
	num1[i] = dis(gen);
	num2[i] = dis(gen);
    }
    num1[99] = 0;
    num2[99] = 0;
    precision p1 = {50, 50};
    workspace space = workspace(p1, "test space");
    if(space.add_block("dummy1", num1, 100, false)) {
	std::cout << "Error block was not added" << "\n";
	return;
    }
    if(space.add_block("dummy2", num2, 100, false)) {
	std::cout << "Error block was not added" << "\n";
	return;
    }
    if (space.add_block("sum", space.add("dummy1", "dummy2")) == 1) {
	std::cout << "Error block was not added" << "\n";
	return;
    }
    blocks sum_b = space.get_block("sum");
    std::vector<int> sum = space.get_block("sum").get_blocks();
    for (auto i : sum) {
	std::cout << i;
    }
    std::cout << "\n";
    std::cout << sum_b.to_string() << "\n";
    
}


int main(int argc, char** argv) {
    if (argc < 4) {
	default_test();
    }
    else {
	blocks sum;
	char* op = argv[2];
	if ((strcmp(op, "+") != 0) && (strcmp(op, "-") != 0)) {
	    std::cout << op << " is not a a valid operation\n";
	    return 0;
	}
	std::string num1 = argv[1];


	std::string num2 = argv[3];
	unsigned max_p = std::max(num1.size(), num2.size());
	precision p = {max_p, max_p};
	blocks b1 = blocks(p, num1);
	blocks b2 = blocks(p, num2);
	if (b1.is_fail_block() || b2.is_fail_block()) {
	    std::cerr << "The creation of the blocks failed.\n";
	    return 0;
	}
	if (strcmp(op, "+") == 0) {
	    sum = b1 + b2;
	}
	else {
	    sum = b1 - b2;
	}
	std::cout << b1.to_string() << " " << op
		  << " " << b2.to_string() << " = " << sum.to_string()
		  << "\n";
	
    }
}
