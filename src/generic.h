#pragma once

#include "debug_msg.h"
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

// Get number of bits per byte
constexpr int BITS_PER_BYTE = std::numeric_limits<unsigned char>::digits;

// Checks if the directory exists and replaces extension bin and txt files.
const std::string sanitize_file_path(std::string&, const std::string&);
