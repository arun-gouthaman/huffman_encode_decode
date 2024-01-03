#pragma once
#include "huffman_tree.h"
#include "generic.h"

// Struct to hold binary data
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

	// Read input from text file
	const std::string read_text_file(const std::string&);

	// Get frequency of each character
	std::vector<std::pair<char, int>> build_frequency_vector(const std::string&);

	// Replace characters with bit codes from huffman tree structure
	const bytes_data_struct build_bits(const std::string&, const std::map<const char, const std::string>);
	
	// Compute array length and padding length for last bitset to pack it with number of bits per byte.
	// ex: 0011 will be written as 00000011 to pack it as 8 bits, padding length will be 4
	int* Encode::bit_array_padding_length();

	// Save binary data
	bool write_binary_file(const bytes_data_struct&, std::string&);

	// Save frequency data to rebuild huffman tree to decode.
	bool write_freq_map_data(const std::vector<std::pair<char, int>>, std::string&);

public:
	void encode(const std::string, std::string);
};