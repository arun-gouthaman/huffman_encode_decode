#include "huffman_tree.h"
#include "debug_msg.h"
#include <iostream>

void reduce_nodes(std::vector<std::unique_ptr<Node>>& freq_vec, bool to_root = false)
{
    DEBUG_MSG("\nDebug: Branching nodes\nReceived nodes count:" << freq_vec.size() << "\n");
    std::vector<std::unique_ptr<Node>>::iterator it = freq_vec.begin();
    std::vector<std::unique_ptr<Node>> ret_vec;
    while (it != freq_vec.end())
    {
        std::unique_ptr<Node> node1 = std::make_unique<Node>();
        std::unique_ptr<Node> node2 = std::make_unique<Node>();
        std::unique_ptr<Node> sum_node = std::make_unique<Node>();
        node1 = std::move(*it);
        ++it;
        if (it != freq_vec.end())
        {
            node2 = std::move(*it);
            sum_node->sum = node1->sum + node2->sum;
            sum_node->l_node = std::move(node1);
            sum_node->r_node = std::move(node2);
            ret_vec.push_back(std::move(sum_node));
        }
        else
        {
            ret_vec.push_back(std::move(node1));
        }
        if (it != freq_vec.end())
        {
            ++it;
        }
    }
    if (to_root && ret_vec.size() > 1)
    {
        reduce_nodes(ret_vec, to_root);
    }
    freq_vec = std::move(ret_vec);
}
//
std::unique_ptr<Node> HuffmanTree::build_tree(std::vector<std::pair<char, int>> freq_vec)
{
    DEBUG_MSG("\nDebug: Building tree\n");
    if (freq_vec.empty())
    {
        return std::unique_ptr<Node>();
    }

    int cur_freq = freq_vec[0].second;

    std::vector<std::unique_ptr<Node>> tmp_vec;
    std::vector<std::pair<char, int>>::iterator it = freq_vec.begin();

    while (it != freq_vec.end())
    {
        std::unique_ptr<Node> tmp_node = std::make_unique<Node>();
        tmp_node->ch = it->first;
        tmp_node->sum = it->second;
        if (it->second == cur_freq)
        {
            tmp_vec.push_back(std::move(tmp_node));
        }
        else
        {
            cur_freq = it->second;
            if (tmp_vec.size() % 2)
            {
                tmp_vec.push_back(std::move(tmp_node));
                reduce_nodes(tmp_vec, false);
            }
            else
            {
                reduce_nodes(tmp_vec, false);
                tmp_vec.push_back(std::move(tmp_node));
            }
        }
        ++it;
    }
    reduce_nodes(tmp_vec, true);
    return std::move(tmp_vec[0]);
}

const std::string HuffmanTree::extract_from_tree(const std::string binary_bits, std::unique_ptr<Node>& node)
{
    DEBUG_MSG("\nDebug: Extracting data from huffman tree\n");
    Node* cur_node = node.get();
    std::string::const_iterator it = binary_bits.begin();
    std::string extracted_str = "";
    while (it != binary_bits.end())
    {
        if (cur_node->ch == '\0')
        {
            if (*it == '1')
            {
                cur_node = cur_node->l_node.get();
            }
            else
            {
                cur_node = cur_node->r_node.get();
            }
        }
        else
        {
            extracted_str += cur_node->ch;
            cur_node = node.get();
            continue;
        }
        ++it;
    }
    extracted_str += cur_node->ch;

    return extracted_str;
}

void HuffmanTree::navigate_from_node(std::unique_ptr<Node>& node, int indent = 1)
{
    std::string indent_char = std::string(indent, ' ');
    if (node->ch != '\0')
    {
        if (node->ch == '\n')
        {
            std::cout << indent_char << "#" << " -> " << node->sum << "\n";
        }
        else
        {
            std::cout << indent_char << node->ch << " -> " << node->sum << "\n";
        }
    }
    else
    {
        std::cout << indent_char << node->sum << "\n";
    }
    if (node->l_node)
    {
        navigate_from_node(node->l_node, indent + 1);
    }
    if (node->r_node)
    {
        navigate_from_node(node->r_node, indent + 1);
    }
}

void HuffmanTree::build_map(std::unique_ptr<Node>& node, std::string bits, std::map<const char, const std::string>& op_map)
{
    if (node->ch != '\0')
    {
        op_map.emplace(node->ch, bits);
        bits_length += bits.length() * node->sum;
    }
    if (node->l_node)
    {
        bits.push_back('1');
        build_map(node->l_node, bits, op_map);
        bits.pop_back();
    }
    if (node->r_node)
    {
        bits.push_back('0');
        build_map(node->r_node, bits, op_map);
        bits.pop_back();
    }
}
