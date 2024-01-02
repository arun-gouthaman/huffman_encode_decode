#pragma once
#include "huffman_tree.h"
#include "generic.h"

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