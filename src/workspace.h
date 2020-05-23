#ifndef SPACE
#define SPACE
#include "blocks.h"
#include <iostream>
#include <string>
#include <map>
typedef std::map<std::string, blocks> blockmap;
class workspace {
private:
    const precision global_precision_;
    const int blocks_size_;
    std::string workspace_id_;
    blockmap collection_;
public:
    // constructors
    workspace(precision p, std::string workspace_id) : global_precision_(p), blocks_size_(p.negative_precision + p.positive_precision)
    {
	workspace_id_ = workspace_id;
    }
    workspace& operator= (workspace& w);
	
    // int add_block(std::string new_name, std::string digits); // return 0 for success 1 for failure
    int add_block(std::string new_name, int number[], int digits_size, bool is_negative);
    int add_block(std::string new_name, blocks b);

    // get
    blocks get_block(std::string name);
    // get workspace info
    int size() {return collection_.size();}
    std::string get_id() {return workspace_id_;}
    std::string get_all_info();
    // operations and algorithms
    blocks add(std::string id1, std::string id2);
    blocks gradeschool_multiply(std::string id1, std::string id2);
    blocks karatsuba(std::string id1, std::string id2);
    blocks divide(std::string id1, std::string id2);
    blocks greater_than(std::string id1, std::string id2); // is id1 > id2
    blocks less_than(std::string id1, std::string id2); // id1 < id2 <=>
    // greater_than(id2, id1)
			
};
#endif
