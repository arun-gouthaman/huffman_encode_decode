#pragma once
#include <string>
#include <map>
#include <vector>
#include "huffman_tree.h"

class Decode
{
private:
	HuffmanTree huffman_tree;

public:
	std::string read_binary_file(std::string);
	const std::vector<std::pair<char, int>> read_freq_file(std::string);
	const std::vector<std::pair<char, int>> parse_freq_data(const std::string&);

	void decode(std::string);
};