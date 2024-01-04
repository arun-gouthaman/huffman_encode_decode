#pragma once
#include<string>
#include <map>
#include<memory>
#include <vector>

struct Node
{
public:
	char ch = '\0';
	int sum = 0;

    std::unique_ptr<Node> l_node;
    std::unique_ptr<Node> r_node;
};

class HuffmanTree
{
private:
    int bits_length = 0;
public:
    // build huffman tree from frequency data
    std::unique_ptr<Node> build_tree(std::vector<std::pair<char, int>>);
    
    
    std::unique_ptr<Node> build_test_tree(std::vector<std::pair<char, int>>);


    // Test function to print tree structure from given node
    void navigate_from_node(std::unique_ptr<Node>&, int);

    // Generate bits for each character based on tree navigation
    void build_map(std::unique_ptr<Node>&, std::string, std::map<const char, const std::string>&);

    // Use bits data to navigate the tree and get characters to decode
    const std::string extract_from_tree(const std::string, std::unique_ptr<Node>&);

    // Get total number of bits generated for all characters.
    int get_bits_length()
    {
        return bits_length;
    }
};