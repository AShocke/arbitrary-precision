#ifndef BLOCKS
#define BLOCKS
#include<iostream>
#include<vector>
#include<string>
struct precision {
    unsigned int negative_precision; // number of blocks (visible or not) that
    // are before the decimal
    unsigned int positive_precision; // number of blocks after the decimal
    precision& operator= (const precision& p1) 
    {
	negative_precision = p1.negative_precision;
	positive_precision = p1.positive_precision;
	return *this;
    }
    bool operator== (const precision& p1) {
	return (negative_precision == p1.negative_precision && positive_precision == p1.positive_precision);
    }
    unsigned int size() {
	return negative_precision + positive_precision;
    }
};
const precision zero_precision = {0, 0};
class blocks {
private:
    /*
     * Numbers are stored in preallocated blocks, with the set of allocated
     *  blocks forming a number called the visible blocks.
     */
    std::vector<int> blocks_; // The main data structure for the blocks
    precision blocks_precision_;
    bool is_negative_;
    bool FAIL_BLOCK_;
    bool DEBUG_;
    int subtract(std::vector<int> s1, std::vector<int> s2, std::vector<int>& sum);// s1 - s2
    bool abs_less_than(const blocks b); // Check for less than in absolute value
public:
    // Constructors
    blocks(){
	blocks_precision_ = zero_precision;
	blocks_ = std::vector<int>(0);
        is_negative_ = false;
	FAIL_BLOCK_ = false;
	
    }
    blocks(const precision p, std::string num);
    blocks(const precision p, std::vector<int> v, bool is_negative = false);
    blocks(const precision p, const int digits[], int digits_size, bool is_negative = false);
    blocks(bool is_fail_block) {FAIL_BLOCK_ = is_fail_block;}
    blocks operator= (const blocks b);
    // Get info
    std::string to_string(int place = -1); //defualt is to remove
					   //trailing zeroes at the
					   //start of the number
    //place is how many decimal places fter the decimal are printed up to negative precision
    void print(int decimal_precision); // gives option of number of places after
    precision get_precision(){return blocks_precision_;}
    // decimal
    int get_size();
    std::vector<int> get_blocks() {return blocks_;}
    bool is_negative() {return is_negative_;}
    bool is_fail_block() { return FAIL_BLOCK_;}
    //operations
    blocks operator+(const blocks b); //adding and subtracting 
    blocks operator*(const blocks b);
    blocks negate() const; // negation 
    blocks operator-(const blocks b);
};
const blocks FAIL_BLOCK = blocks(true);

#endif
