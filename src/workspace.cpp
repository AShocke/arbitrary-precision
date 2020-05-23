#include "workspace.h"

/*
int workspace::add_block(std::string name, std::string digits) {
    if (collection_.find(name) == collection_.end()) {
	blocks new_blocks = blocks(digits);
	collection_[name] = new_blocks;	
	return 0; // maybe try-catch?
    }
    else {
	return 1;
    }
}
*/
int workspace::add_block(std::string name, int digits[], int digits_size, bool is_negative) {
    if (collection_.find(name) == collection_.end()) {
	blocks new_blocks = blocks(global_precision_, digits, digits_size, is_negative);
	collection_[name] = new_blocks;
	return 0;
    }
    else {
	return 1;
    }
}
int workspace::add_block(std::string name, blocks b) {
    if (collection_.find(name) == collection_.end()) {
	collection_[name] = b;
	return 0;
    }
    else {
	return 1;
    }
}

// Get
blocks workspace::get_block(std::string name) {
    if (collection_.find(name) == collection_.end()) {
	return blocks(true);
    }
    else {
	return collection_[name];
    }
}
// Operations
blocks workspace::add(std::string id1, std::string id2) {
    bool fail_flag = false;
    int carry = 0;
    if(collection_.find(id1) == collection_.end()) {
	std::cerr << "Symbol: " << "`" << id1 << "'" << "does not exist.\n";
	fail_flag = true;
    }
    if (collection_.find(id2) == collection_.end()) {
	std::cerr << "Symbol: " << "`" << id2 << "'" << "does not exist.\n";
	fail_flag = true;
    }
    if (fail_flag) {
	return blocks(true); // return fail block
    }
    bool is_negative1 = collection_[id1].is_negative();
    bool is_negative2 = collection_[id2].is_negative();
    if (!(is_negative1 && is_negative2) && (is_negative1 || is_negative2)) { // if not both true and not both false we "subtract"
	//TODO
	return blocks(true); // *** !!!!! Change this later
    }
    else {
	std::vector<int> num1 = collection_[id1].get_blocks();
	std::vector<int> num2 = collection_[id2].get_blocks();
	std::vector<int> sum = std::vector<int>(blocks_size_);
	for (int i = 0; i < blocks_size_; i++) {
	    sum[i] = (num1[i] + num2[i] + carry) % 10 ;
	    carry = (num1[i] + num2[i]) / 10;
	}
	if (carry > 0) {
	    std::cerr << "Overflow error. Result is likely inaccurate.\n";
	}
	blocks sum_blocks = blocks(global_precision_, sum);
	return sum_blocks;
    }
}
