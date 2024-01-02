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
    std::unique_ptr<Node> build_tree(std::vector<std::pair<char, int>>);
    void navigate_from_node(std::unique_ptr<Node>&, int);
    void build_map(std::unique_ptr<Node>&, std::string, std::map<const char, const std::string>&);
    const std::string extract_from_tree(const std::string, std::unique_ptr<Node>&);
    int get_bits_length()
    {
        return bits_length;
    }
};