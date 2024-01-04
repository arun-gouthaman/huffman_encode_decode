#include "file_encode.h"

#include <algorithm>
#include <filesystem>



void Encode::encode(const std::string ip_file_path, std::string op_file_path)
{
	std::cout <<"\nEncoding...\n";
	const std::string& file_content = read_text_file(ip_file_path);
	std::vector<std::pair<char, int>> freq_vec = build_frequency_vector(file_content);

	std::map<const char, const std::string> op_map = {};

	std::unique_ptr<Node> root = huffman_tree.build_tree(freq_vec);

	huffman_tree.build_map(root, std::string(""), op_map);

	const bytes_data_struct byte_pack_data = build_bits(file_content, op_map);

	write_binary_file(byte_pack_data, op_file_path);
	write_freq_map_data(freq_vec, op_file_path);
}

const std::string Encode::read_text_file(const std::string& file_path)
{
	DEBUG_MSG("\nDebug: Reading from file " << file_path << "\n");
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

// Lambda function to sort vector<pair<char, int>> based on the int values.
auto compare_pair = [](std::pair<char, int> a, std::pair<char, int> b) { return a.second < b.second; };

std::vector<std::pair<char, int>> Encode::build_frequency_vector(const std::string& file_data)
{
	DEBUG_MSG("\nDebug: Building frequency data\n");
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

	// Sort vector in order of frequency to build huffman tree
	// characters with high frequency appears near the top of the tree
	std::sort(sort_vec.begin(), sort_vec.end(), compare_pair);

	return sort_vec;
}

int* Encode::bit_array_padding_length()
{
	DEBUG_MSG("\nDebug: Calculating padding bits length \n");
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
	else if ((bits_length % BITS_PER_BYTE) && bits_length > BITS_PER_BYTE)
	{
		padding_length = BITS_PER_BYTE - (bits_length % BITS_PER_BYTE);
	}

	int* len_info = new int[2]{ array_len, padding_length };
	return len_info;

}

const bytes_data_struct Encode::build_bits(const std::string& ip_data, const std::map<const char, const std::string> bit_map)
{
	DEBUG_MSG("\nDebug: Building bits\n");
	std::string bit_string = std::string();

	int* len_data = bit_array_padding_length();
	int array_len = len_data[0];
	int padding_len = len_data[1];
	delete len_data;
	len_data = nullptr;

	std::bitset<BITS_PER_BYTE>* bitset_array = new std::bitset<BITS_PER_BYTE>[array_len];
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
			DEBUG_MSG("Debug: bit string:" << bit_string);
			bit_string.clear();
			++cur_index;
		}
		else if (bit_string.length() > BITS_PER_BYTE)
		{
			while (bit_string.length() > BITS_PER_BYTE)
			{
				bitset_array[cur_index] = std::bitset<BITS_PER_BYTE>(bit_string.substr(0, BITS_PER_BYTE));
				DEBUG_MSG("Debug: bit string:" << bit_string.substr(0, BITS_PER_BYTE));
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
		DEBUG_MSG("Debug: bit string:" << bit_string);
	}

	bytes_data_struct bytes_data;

	bytes_data.array_len = array_len;
	bytes_data.padding_len = padding_len;
	bytes_data.bit_set_array = bitset_array;
	DEBUG_MSG("Debug: Padding Length: " << bytes_data.padding_len << "\n");
	return bytes_data;
}

bool Encode::write_binary_file(const bytes_data_struct& byte_data, std::string& write_file_path)
{
	DEBUG_MSG("\nDebug: Writing to binary file " << write_file_path << "\n");
	write_file_path = sanitize_file_path(write_file_path, "bin");

	// Delete file if exists before writing.
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
		DEBUG_MSG("\nDebug: Writing byte: " << byte_data.bit_set_array[i] << "\n");
		const char uc = (byte_data.bit_set_array[i]).to_ulong();
		write_to_file.write(reinterpret_cast<const char*>(&uc), 1);
	}

	write_to_file.close();

	return true;
}

bool Encode::write_freq_map_data(const std::vector<std::pair<char, int>> freq_vec, std::string& write_file_path)
{
	write_file_path = sanitize_file_path(write_file_path, "txt");
	DEBUG_MSG("\nDebug: Writing frequency map to " << write_file_path << "\n");

	std::ofstream write_to_file(write_file_path, std::ios::out);
	if (!write_to_file)
	{
		return false;
	}

	std::string mapping_data_string = std::string("");

	// Build data as key value structure
	// [key1:value1][key2:value2]
	for (const std::pair<char, int> p : freq_vec)
	{
		mapping_data_string += "[" + std::string(1,p.first) + ":" + std::to_string(p.second) + "]";
	}

	write_to_file << mapping_data_string;

	write_to_file.close();

	return true;
}
