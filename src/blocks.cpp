#include "blocks.h"

blocks::blocks(const precision p, std::string num) {
   
    if (num.size() == 0) {
	blocks_precision_ = zero_precision;
	blocks_ = std::vector<int>(0);
        is_negative_ = false;
	FAIL_BLOCK_ = false;
	return;
    }
    else {
	is_negative_ = false;
	if (num[0] == '-') {
	    is_negative_ = true;
	    num = num.substr(1);
	}
	blocks_precision_ = p;
	std::size_t decimal= num.find(".");
	// Case no decimal point, entire number is in the positive magnitudes
        if (decimal == std::string::npos) {
	    if (num.size() > p.positive_precision) {
		FAIL_BLOCK_ = true;
		return;
	    }
	    else {
		blocks_ = std::vector<int>(p.positive_precision + p.negative_precision, 0);
		for (int i = 0; i < num.size(); i++) {
		    char c = num[num.size() - 1- i];
		    if (c < '0' || c > '9') {
			std::cerr << "Bad input.\n";
			FAIL_BLOCK_ = true;
		    }
		    blocks_[p.negative_precision + i] =  c - '0'; // Char convertion	 
		}
	    }
	}
	// Case decimal point is at the start of string
	else if (decimal == 0) {
	    std::string B = num.substr(1);
	    if (B.size() > p.negative_precision) {
		FAIL_BLOCK_ = true;
		return;
	    }
	    else {
		blocks_ = std::vector<int>(p.positive_precision + p.negative_precision, 0);
		for (int i = 0; i < B.size(); i++) {
		    char c = B[B.size() - 1 -i];
		    if (c < '0' || c > '9') {
			std::cerr << "Bad input.\n";
			FAIL_BLOCK_ = true;
			return;
		    }
		    blocks_[p.negative_precision - B.size() + i] = c - '0'; //Char conversion
		}
	    }
	}
	// Standard case, decimal point somewhere in the middle
	else {
	    std::string F = num.substr(0, decimal);
	    std::string B = num.substr(decimal + 1);
	    if (F.size() > p.positive_precision || B.size() > p.negative_precision) {
		std::cerr << "Given number violates given precision.\n";
		FAIL_BLOCK_ = true;
		return;
	    }
	    blocks_ = std::vector<int>(p.positive_precision + p.negative_precision, 0);
	    for (unsigned int i = 0; i < B.size(); i++) {
		char c = B[B.size() - 1 - i];
		if (c < '0' || c > '9') {
		    std::cerr << "Bad input.\n";
		    FAIL_BLOCK_ = true;
		    return;
		}
		blocks_[p.negative_precision - B.size() + i] = c - '0';
	    }
	    for (unsigned int i = 0; i < F.size(); i++) {
		char c = F[F.size() - 1 - i];
		if (c < '0' || c > '9') {
		    std::cerr << "Bad input.";
		    FAIL_BLOCK_ = true;
		    return;
		}
		blocks_[p.negative_precision + i] = c - '0'; // Char conversion
	    }
	}
    }
}
blocks::blocks(const precision p, std::vector<int> v, bool is_negative) {
    if (p.negative_precision + p.positive_precision != v.size()) {
	std::cerr << "The given number violates the given precision\n";
	FAIL_BLOCK_ = true;
	return;
    }
    else {
	blocks_precision_ = p;
	blocks_ = v;
	is_negative_ = is_negative;
	FAIL_BLOCK_ = false;
    }
}

blocks::blocks(const precision p, const int digits[], int digits_size, bool is_negative) { //assume digits_size is correct
    if (p.negative_precision + p.positive_precision != digits_size) {
       	std::cerr << "The given number violates the given precision\n";
	FAIL_BLOCK_ = true;
	return;
    }
    else {
	blocks_precision_ = p;
	is_negative_ = is_negative;
	blocks_ = std::vector<int>(digits_size);
	for (int i = 0; i < digits_size; i++) {
	    if (digits[i] >= 10 || digits[i] < 0) {
		FAIL_BLOCK_ = true;
		std::cerr << "A digit was out of bounds\n";
		return;
	    }
	    blocks_[i] = digits[i];
	}
	FAIL_BLOCK_ = false;
    }
}

blocks blocks::operator=(const blocks b) {
    if (b.FAIL_BLOCK_) {
	std::cerr << "A FAIL_BLOCK cannot be equated\n";
	return *this;
    }
    else {
	blocks_ = b.blocks_;
	blocks_precision_ = b.blocks_precision_;
	is_negative_ = b.is_negative_;

	return *this;
    }
}
// Remove Trailing zeroes
std::string blocks::to_string(int place) {
    std::string num = "";
    bool trailing_zero = true;
    int num_trail = 0;
    if (is_negative_) {
	num+= "-";
    }
    for (int i = blocks_.size() - 1; i >= (int) (blocks_precision_.negative_precision); i--) {
	if (blocks_[i] != 0) {
	    trailing_zero = false;
	}
	if (!trailing_zero) {
	    num += std::to_string(blocks_[i]);
	}
    }
    num += ".";
    if (place < 0) {
	trailing_zero = true;
	unsigned i = 0;
	while (i < blocks_precision_.negative_precision && trailing_zero) {
	    if (blocks_[i] == 0) {
		num_trail++;
	    }
	    else {
		trailing_zero = false;
	    }
	    i++;
	}
	for (int i = 0; i < (int)(blocks_precision_.negative_precision - num_trail); i++) {
	    num += std::to_string(blocks_[blocks_precision_.negative_precision - 1 - i]);
	}
    }
    else {
	if (place > blocks_precision_.negative_precision) {
	    place = blocks_precision_.negative_precision;
	}
	for (int i = 0; i < place; i++) {
	    num += std::to_string(blocks_[blocks_precision_.negative_precision- 1 -i]);
	}
    }
    return num;
}

// operations

/* Compute num1 - num2. Always assuming that num1 > num2 in absolute value
 */
int blocks::subtract(std::vector<int> num1, std::vector<int> num2,
		     std::vector<int>& diff) 
{
    int carry = 0;
    int blocks_size = num1.size();;
    for (int i = 0; i < blocks_size; i++) {
	if (num1[i] == 0 && carry == -1) {
	    diff[i] = 9  - num2[i];
	}
	if (num1[i] + carry < num2[i]) {
	    diff[i] = 10 + num1[i] + carry - num2[i];
	    carry = - 1;
	}
	else {
	    diff[i] = num1[i] + carry - num2[i];
	    carry = 0;
	}
    }
    return carry;
}

blocks blocks::operator+(const blocks b) {
    if (FAIL_BLOCK_ || b.FAIL_BLOCK_) {
	std::cerr << "Cannot do operations on fail state blocks\n";
	return FAIL_BLOCK;
    }
    else if (!(blocks_precision_ == b.blocks_precision_)) {
	std::cerr << "Blocks are not of the same precision.\n";
	return FAIL_BLOCK;
    }
    else if ((is_negative_ && b.is_negative_) || (!is_negative_ && !b.is_negative_))
	{
	    int carry = 0;
	    int blocks_size = blocks_precision_.size();
	    std::vector<int> num1 = blocks_;
	    std::vector<int> num2 = b.blocks_;
	    std::vector<int> sum = std::vector<int>(blocks_size);
	    for (int i = 0; i < blocks_size; i++) {
		sum[i] = (num1[i] + num2[i] + carry) % 10 ;
		carry = (num1[i] + num2[i]) / 10;
	    }
	    if (carry > 0) {
		std::cerr << "Overflow error. Result is likely inaccurate.\n";
	    }
	    blocks sum_blocks = blocks(blocks_precision_, sum, is_negative_);
	    if (sum_blocks.is_fail_block()) {
		return FAIL_BLOCK;
	    }
	    else {
		return sum_blocks;
	    }
	} 
    else {
	int blocks_size = blocks_precision_.size();
	std::vector<int> num1 = blocks_;
	std::vector<int> num2 = b.blocks_;
	std::vector<int> sum = std::vector<int>(blocks_size);
	bool is_sum_negative;
	/* There are four cases in the gradeschool subtracting
	 algorithm Note that we cannot, for example, use the
	 gradeschool method to subtract 1 - 12 using gradeschool
	 method (we cannot subtract smaller from a larger number in
	 absolute value) but must instead compute 12 - 1 and then use
	 1 - 12 = -(12 - 1)
	*/ 
	if (is_negative_) {
	    bool neg_abs = abs_less_than(b);
	    is_sum_negative = !neg_abs;

	    // *this is less than second in absolute value so we subtract as normal
	    if (abs_less_than(b)) {
		subtract(num2, num1, sum);
	    }
	    // must utilize num2 - num1 = -(num1 - num2)
	    else {
		subtract(num1, num2, sum);		
	    }
	}
	else {
	    bool neg_abs = abs_less_than(b);
	    is_sum_negative = neg_abs; // sum *this + b is negative if |*this| < |b|
	    //and b is negative
	     // *this is less than second in absolute value so we subtract as normal
	    if (neg_abs) {
		subtract(num2, num1, sum);
	    }
	    // must utilize num2 - num1 = -(num1 - num2)
	    else {
		subtract(num1, num2, sum);		
	    }	    
	}
	blocks sum_blocks = blocks(blocks_precision_, sum, is_sum_negative);
	if (sum_blocks.is_fail_block()) {
	    return FAIL_BLOCK;
	}
	else {
	    return sum_blocks;
	}
    }
}
blocks blocks::operator-(const blocks b) {
    blocks neg_b = b.negate();
    std::cout << b.is_negative_ << "\n";
    return *this + neg_b;
}
blocks blocks::negate() const {
    blocks b = *this;
    b.is_negative_ = !is_negative_;
    return b;
}
bool blocks::abs_less_than(const blocks b) {
    if (!(blocks_precision_ == b.blocks_precision_)) {
	std::cerr << "Error: can only compare blocks of the same precision";
	return false;
    }
    else {
	std::vector<int> n1 = blocks_;
	std::vector<int> n2 = b.blocks_;
	for (int i = blocks_.size() - 1; i >= 0; i--) {
	    if (n1[i] > n2[i]) {
		return false;
	    }
	    if (n2[i] > n1[i]) {
		return true;
	    }
	}
	return false; // R
    }
}
