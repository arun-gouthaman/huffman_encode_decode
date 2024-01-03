#pragma once
#include "huffman_tree.h"
#include "generic.h"

class Decode
{
private:
	HuffmanTree huffman_tree;

private:
	// Read bits from binary file
	std::string read_binary_file(std::string);

	// Read character frequenct information to build huffman tree
	const std::vector<std::pair<char, int>> read_freq_file(std::string);

	// Parse frequency data to std::vector<std::pair<char, int>>
	const std::vector<std::pair<char, int>> parse_freq_data(const std::string&);
public:
	void decode(std::string, std::string);
};