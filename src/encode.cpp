#include "encode.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <sstream>


const std::string Encode::read_text_file(const std::string& file_path)
{
	std::string file_data;
	std::ifstream input_data(file_path, std::ios::in);
	if (!input_data)
	{
		return std::string();
	}
	std::string content((std::istreambuf_iterator<char>(input_data)),
		(std::istreambuf_iterator<char>()));
	return content;
}

auto compare_pair = [](std::pair<char, int> a, std::pair<char, int> b) { return a < b; };

std::vector<std::pair<char, int>> Encode::build_frequency_vector(const std::string& file_data)
{
	std::map<char, int> freq_map;
	std::string::const_iterator str_it = file_data.begin();
	while (str_it != file_data.end())
	{
		freq_map[*str_it] += 1;
		str_it++;
	}

	std::vector<std::pair<char, int>> sort_vec;
	for (std::pair<char, int> p : freq_map)
	{
		sort_vec.push_back(p);
	}

	std::sort(sort_vec.begin(), sort_vec.end(), compare_pair);

	return sort_vec;
}

void Encode::encode(const std::string ip_file_path, std::string op_file_path)
{
	const std::string& file_content = read_text_file(ip_file_path);
	std::vector<std::pair<char, int>> freq_vec = build_frequency_vector(file_content);

	for (std::pair<const char, const int> p : freq_vec)
	{
		std::cout << p.first << " : " << p.second << "\n";
	}

	std::unique_ptr<Node> root = huffman_tree.build_tree(freq_vec);
	//std::unique_ptr<Node> root = huffman_tree.new_create_tree(freq_vec);


	std::cout << "Root test : " << root->sum << "\n";

	huffman_tree.navigate_from_node(root, 1);
	std::map<const char, const std::string> op_map = {};
	huffman_tree.build_map(root, std::string(""), op_map);

	std::cout << "\nop map test\n";
	for (std::pair<const char, const std::string> p : op_map)
	{
		std::cout << p.first << " : " << p.second << "\n";
	}

	std::cout << "\nBits length\n" << huffman_tree.get_bits_length();

	const bytes_data_struct byte_pack_data =  build_bits(file_content, op_map);

	for (int i = 0; i < byte_pack_data.array_len; ++i)
	{
		std::cout << "--------" << byte_pack_data.bit_set_array[i] << "\n";
	}

	write_binary_file(byte_pack_data, op_file_path);
	write_freq_map_data(freq_vec, op_file_path);
}

int* Encode::bit_array_padding_length()
{
	int bits_length = huffman_tree.get_bits_length();
	int array_len = 1;
	int padding_length = 0;

	array_len = std::max((bits_length / BITS_PER_BYTE), array_len);
	if (bits_length > BITS_PER_BYTE && bits_length % BITS_PER_BYTE)
	{
		// Increase array length if number of bits 
		// are more than 8 and not exactly divisible by 8
		// to compensate for excess bits at end after packing as set of 8.
		++array_len;
	}
	// Incrementing array length to hold padding digits count
	++array_len;

	if (bits_length < BITS_PER_BYTE)
	{
		padding_length = BITS_PER_BYTE - bits_length;
	}
	else if (bits_length > BITS_PER_BYTE)
	{
		padding_length = BITS_PER_BYTE - (bits_length % BITS_PER_BYTE);
	}

	int* len_info = new int[2]{ array_len, padding_length };
	return len_info;

}

const bytes_data_struct Encode::build_bits(const std::string& ip_data, const std::map<const char, const std::string> bit_map)
{
	std::cout << "\nBuilding bits\n";
	std::string bit_string = std::string();

	int* len_data = bit_array_padding_length();
	int array_len = len_data[0];
	int padding_len = len_data[1];
	delete len_data;
	len_data = nullptr;

	std::bitset<8>* bitset_array = new std::bitset<8>[array_len];
	bitset_array[0] = std::bitset<BITS_PER_BYTE>(padding_len);
	
	int cur_index = 1;
	std::string::const_iterator it = ip_data.begin();
	while (it != ip_data.end())
	{
		if (bit_map.find(*it) != bit_map.end())
		{
			bit_string += bit_map.find(*it)->second;
		}
		if (bit_string.length() == BITS_PER_BYTE)
		{
			bitset_array[cur_index] = std::bitset<BITS_PER_BYTE>(bit_string);
			bit_string.clear();
			++cur_index;
		}
		else if (bit_string.length() > BITS_PER_BYTE)
		{
			while (bit_string.length() > BITS_PER_BYTE)
			{
				bitset_array[cur_index] = std::bitset<BITS_PER_BYTE>(bit_string.substr(0, BITS_PER_BYTE));
				bit_string = bit_string.substr(BITS_PER_BYTE);
				++cur_index;
			}
		}
		++it;
	}
	if (!bit_string.empty())
	{
		bit_string = std::string(padding_len, '0') + bit_string;
		bitset_array[cur_index] = std::bitset<BITS_PER_BYTE>(bit_string);
	}

	bytes_data_struct bytes_data;

	bytes_data.array_len = array_len;
	bytes_data.padding_len = padding_len;
	bytes_data.bit_set_array = bitset_array;

	return bytes_data;
}

const std::string satinize_file_path(std::string& file_path, const std::string& ext)
{

	std::filesystem::path file_sys_path = std::filesystem::path(file_path);

	if (!std::filesystem::is_directory(file_sys_path.parent_path()))
	{
		std::cout << "Invalid Directory " << file_sys_path.parent_path() << "\n";
		return std::string("");
	}

	if (!ext.empty())
	{
		file_sys_path.replace_extension(ext);
	}

	return file_sys_path.string();
}

bool Encode::write_binary_file(const bytes_data_struct& byte_data, std::string& write_file_path)
{

	write_file_path = satinize_file_path(write_file_path, "bin");

	if (std::filesystem::exists(write_file_path))
	{
		std::filesystem::remove(write_file_path);
	}
	std::ofstream write_to_file(write_file_path, std::ios::out | std::ios::binary | std::ios::app);

	if (!write_to_file)
	{
		return false;
	}
	for (int i = 0; i < byte_data.array_len; ++i)
	{
		std::cout << "BITSET TEST: " << byte_data.bit_set_array[i] << "\n";
		const char uc = (byte_data.bit_set_array[i]).to_ulong();
		write_to_file.write(reinterpret_cast<const char*>(&uc), 1);
	}

	write_to_file.close();

	return true;
}

bool Encode::write_freq_map_data(const std::vector<std::pair<char, int>> freq_vec, std::string& write_file_path)
{
	std::cout << "writing frequency map\n";
	write_file_path = satinize_file_path(write_file_path, "txt");

	std::ofstream write_to_file(write_file_path, std::ios::out);
	if (!write_to_file)
	{
		return false;
	}

	std::string mapping_data_string = std::string("");

	for (const std::pair<char, int> p : freq_vec)
	{
		mapping_data_string += "[" + std::string(1,p.first) + ":" + std::to_string(p.second) + "]";
	}

	std::cout << mapping_data_string << "\n";

	write_to_file << mapping_data_string;

	write_to_file.close();

	return true;
}
