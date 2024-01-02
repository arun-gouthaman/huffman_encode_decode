#pragma once
#include <string>
#include <vector>
#include <bitset>
#include <memory>

#include "huffman_tree.h"
#include "source.h"

struct bytes_data_struct
{
	std::bitset<BITS_PER_BYTE>* bit_set_array;
	int array_len = 1;
	int padding_len = 0;
};

class Encode
{
private:
	int bytes_array_len = 0;
	int padding_len = 0;

	HuffmanTree huffman_tree;

	const bytes_data_struct build_bits(const std::string&, const std::map<const char, const std::string>);
	const std::string read_text_file(const std::string&);
	std::vector<std::pair<char, int>> build_frequency_vector(const std::string&);
	int* Encode::bit_array_padding_length();

	bool write_binary_file(const bytes_data_struct&, std::string&);
	bool write_freq_map_data(const std::vector<std::pair<char, int>>, std::string&);

public:
	void encode(const std::string, std::string);
};