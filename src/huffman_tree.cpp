#include "huffman_tree.h"
#include "debug_msg.h"
#include <iostream>

std::unique_ptr<Node> HuffmanTree::build_tree(std::vector<std::pair<char, int>> ip_node_vec)
{
    DEBUG_MSG("\nDebug: Building Huffman Tree\n");
    int vec_size = ip_node_vec.size();

    std::unique_ptr<Node> ref_node = nullptr;

    for (int i = 0; i < vec_size;)
    {
        std::unique_ptr<Node> node1 = std::make_unique<Node>();
        node1->ch = ip_node_vec[i].first;
        node1->sum = ip_node_vec[i].second;
        std::unique_ptr<Node> sum_node = std::make_unique<Node>();

        sum_node->sum = node1->sum;
        sum_node->l_node = std::move(node1);
        node1.reset();
        

        if (i <= vec_size - 2)
        {
            std::unique_ptr<Node> node2 = std::make_unique<Node>();
            node2->ch = ip_node_vec[i + 1].first;
            node2->sum = ip_node_vec[i + 1].second;
            sum_node->sum += node2->sum;
            sum_node->r_node = std::move(node2);
            node2.reset();
        }
        else
        {
            if (ref_node)
            {
                sum_node->sum += ref_node->sum;
                sum_node->r_node = std::move(ref_node);
            }
            ref_node = std::move(sum_node);
            ++i;
            continue;
        }

        if (!ref_node)
        {
            ref_node = std::move(sum_node);
        }
        else
        {
            std::unique_ptr<Node> top_sum = std::make_unique<Node>();
            top_sum->sum = sum_node->sum + ref_node->sum;
            if (sum_node->sum >= ref_node->sum)
            {
                top_sum->l_node = std::move(sum_node);
                top_sum->r_node = std::move(ref_node);
            }
            else
            {
                top_sum->l_node = std::move(ref_node);
                top_sum->r_node = std::move(sum_node);
            }
            ref_node = std::move(top_sum);
        }
        i += 2;
    }

    return ref_node;
}



void reduce_nodes(std::vector<std::unique_ptr<Node>>& freq_vec, bool to_root = false)
{
    std::vector<std::unique_ptr<Node>>::iterator it = freq_vec.begin();
    std::vector<std::unique_ptr<Node>> ret_vec;
    std::cout << "Reducing Nodes\n";
    while (it != freq_vec.end())
    {
        std::unique_ptr<Node> node1 = std::make_unique<Node>();
        std::unique_ptr<Node> node2 = std::make_unique<Node>();
        std::unique_ptr<Node> sum_node = std::make_unique<Node>();
        node1 = std::move(*it);
        std::cout << "Node1: " << node1->ch << "\n";
        ++it;
        if (it != freq_vec.end())
        {
            node2 = std::move(*it);
            std::cout << "Node2: " << node2->ch << "\n";
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
std::unique_ptr<Node> HuffmanTree::build_test_tree(std::vector<std::pair<char, int>> freq_vec)
{
    if (freq_vec.empty())
    {
        return std::unique_ptr<Node>();
    }
    //std::cout << "\nTest Tree\n";
    //for (std::pair<char, int> p : freq_vec)
    //{
    //    std::cout << p.first << ":" << p.second << "\n";
    //}
    std::cout << "Count: " << freq_vec.size() << "\n";

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
