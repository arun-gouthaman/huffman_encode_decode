#include "huffman_tree.h"
#include <iostream>



void reduce_nodes(const std::vector<std::unique_ptr<Node>>& ip_node_vec, std::unique_ptr<Node>& ret_node)
{
    //std::unique_ptr<Node> ret_node = std::make_unique<Node>();
    std::vector<std::unique_ptr<Node>> tmp_vec;
    std::vector<std::unique_ptr<Node>>::const_iterator it = ip_node_vec.begin();
    while (it != ip_node_vec.end())
    {
        std::unique_ptr<Node> node1 = std::make_unique<Node>();
        node1->ch = (*it)->ch;
        node1->sum = (*it)->sum;
        node1->l_node = std::move((*it)->l_node);
        node1->r_node = std::move((*it)->r_node);
        ++it;
        if (it != ip_node_vec.end())
        {
            std::unique_ptr<Node> node2 = std::make_unique<Node>();
            node2->ch = (*it)->ch;
            node2->sum = (*it)->sum;
            node2->l_node = std::move((*it)->l_node);
            node2->r_node = std::move((*it)->r_node);
            std::unique_ptr<Node> tmp_node = std::make_unique<Node>();
            tmp_node->sum = node1->sum + node2->sum;
            tmp_node->l_node = std::move(node1);
            tmp_node->r_node = std::move(node2);
            tmp_vec.push_back(std::move(tmp_node));
            ++it;
        }
        else
        {
            tmp_vec.push_back(std::move(node1));
        }
    }
    if (tmp_vec.size() > 1)
    {
        reduce_nodes(tmp_vec, ret_node);
    }
    else
    {
        ret_node->sum = tmp_vec[0]->sum;
        ret_node->ch = tmp_vec[0]->ch;
        ret_node->l_node = std::move(tmp_vec[0]->l_node);
        ret_node->r_node = std::move(tmp_vec[0]->r_node);
    }

    //return ret_node;
}

std::unique_ptr<Node> HuffmanTree::new_create_tree(std::vector<std::pair<char, int>> sorted_vec)
{
    std::vector<std::pair<char, int>>::iterator it = sorted_vec.begin();
    int cur_freq = it->second;
    std::unique_ptr<Node> ref_node = std::make_unique<Node>();
    std::vector<std::unique_ptr<Node>> nodes_vec;
    while (it != sorted_vec.end())
    {
        std::unique_ptr<Node> cur_node1 = std::make_unique<Node>();
        cur_node1->ch = it->first;
        cur_node1->sum = it->second;
        if (it->second != cur_freq)
        {
            reduce_nodes(nodes_vec, ref_node);
            nodes_vec.clear();
            nodes_vec.push_back(std::move(ref_node));
            int cur_freq = it->second;
            ref_node = std::make_unique<Node>();
        }
        nodes_vec.push_back(std::move(cur_node1));
        ++it;
    }
    reduce_nodes(nodes_vec, ref_node);

    return ref_node;
    return std::unique_ptr<Node>();
}





std::unique_ptr<Node> HuffmanTree::build_tree(std::vector<std::pair<char, int>> ip_node_vec)
{
    std::cout << "Building Huffman Tree\n";
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
        

        if (i <= vec_size - 2)
        {
            std::unique_ptr<Node> node2 = std::make_unique<Node>();
            node2->ch = ip_node_vec[i + 1].first;
            node2->sum = ip_node_vec[i + 1].second;
            sum_node->sum += node2->sum;
            sum_node->r_node = std::move(node2);
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

const std::string HuffmanTree::extract_from_tree(const std::string binary_bits, std::unique_ptr<Node>& node)
{
    std::cout << "Extracting data from huffman tree\n";
    Node* cur_node = node.get();
    std::string::const_iterator it = binary_bits.begin();
    std::string extracted_str = "";
    while (it != binary_bits.end())
    {
        //std::cout << *it;
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
            //std::cout << "\n";
            continue;
        }
        ++it;
    }
    extracted_str += cur_node->ch;
    std::cout << extracted_str;
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
    //if (node->ch == '\n')
    //{
    //    std::cout << "#" << " : " << node->sum << " :: " << bits << "\n";
    //}
    //else
    //{
    //    std::cout << node->ch << " : " << node->sum << " :: " << bits << "\n";
    //}

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
