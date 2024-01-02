#pragma once

#include<limits>
#include <string>
#include<filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <bitset>
#include <sstream>
#include <fstream>


constexpr int BITS_PER_BYTE = std::numeric_limits<unsigned char>::digits;


const std::string sanitize_file_path(std::string&, const std::string&);
