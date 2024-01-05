#include "file_decode.h"


std::string Decode::read_binary_file(std::string file_path)
{
    file_path = sanitize_file_path(file_path, "bin");
    DEBUG_MSG("\nDebug: Reading binary file from " << file_path << "\n");
    std::ifstream read_from_file(file_path, std::ios::binary | std::ios::in);

    if (!read_from_file)
    {
        std::cout << "Error reading file " << file_path << "\n";
        return std::string();
    }

    std::string first_byte = "";
    std::string data_byte = "";
    char c;
    while (read_from_file.get(c))
    {
        for (int i = BITS_PER_BYTE - 1; i >= 0; --i)
        {
            if (first_byte.length() < BITS_PER_BYTE)
            {
                first_byte += std::to_string((c >> i) & 1);
                continue;
            }
            data_byte += std::to_string((c >> i) & 1);
        }
    }
    std::bitset<BITS_PER_BYTE> padding_bits = std::bitset<BITS_PER_BYTE>(first_byte);

    std::string original_data = "";
    original_data = data_byte.substr(0, data_byte.length() - BITS_PER_BYTE);
    std::string end_data = data_byte.substr(data_byte.length() - BITS_PER_BYTE);
    end_data = end_data.substr(padding_bits.to_ulong());
    original_data += end_data;

    return original_data;
}



const std::vector<std::pair<char, int>> Decode::read_freq_file(std::string freq_file_path)
{
    freq_file_path = sanitize_file_path(freq_file_path, "txt");
    DEBUG_MSG("\nDebug: Reading frequency data from " << freq_file_path << "\n");
    std::ifstream read_file(freq_file_path, std::ios::in);
    if (!read_file)
    {
        std::cout << "Error whiler eading file " << freq_file_path << "\n";
        return std::vector<std::pair<char, int>>();
    }

    std::stringstream buffer;
    buffer << read_file.rdbuf();

    std::string frequency_str = buffer.str();

    return parse_freq_data(frequency_str);
}

const std::vector<std::pair<char, int>> Decode::parse_freq_data(const std::string& freq_data_str)
{
    DEBUG_MSG("\nDebug: Parsing frequency data to std::vector<std::pair<char, int>>\n");
    int freq = 0;
    bool open = false;
    std::string cur_str = "";
    std::vector<std::pair<char, int>> freq_vec;
    for (char c : freq_data_str)
    {
        if(c == '[')
        {
            if (open)
            {
                cur_str += c;
                continue;
            }
            open = true;

            if (!cur_str.empty())
            {
                freq_vec.push_back(std::pair<char, int>{cur_str[0], std::stoi(cur_str.substr(2))});
                cur_str.clear();
            }

            continue;
        }
        if (c == ']')
        {
            if (open)
            {
                open = false;
                continue;
            }
            else
            {
                cur_str += ']';
                continue;
            }
        }
        if (!open)
        {
            cur_str +=']';
            open = true;
        }
        cur_str += c;
    }

    if (!cur_str.empty())
    {
        freq_vec.push_back(std::pair<char, int>{cur_str[0], std::stoi(cur_str.substr(2))});
        cur_str.clear();
    }
    return freq_vec;
}



void Decode::decode(std::string binary_file_path, std::string freq_file)
{
    std::cout << "\nDecoding...\n";
    const std::string binary_string = read_binary_file(binary_file_path);
    const std::vector<std::pair<char, int>> freq_vec = read_freq_file(freq_file);
    std::shared_ptr<Node> root = huffman_tree.build_tree(freq_vec);
    //huffman_tree.navigate_from_node(root, 1);
    std::string extracted_string = huffman_tree.extract_from_tree(binary_string, root);
    std::cout << "\nDecoded string:\n" << extracted_string << "\n";
    
}
