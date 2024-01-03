//#include "source.h"
#include "file_encode.h"
#include "file_decode.h"


int main()
{
	Encode encode_file;
	Decode decode_file;
	
	std::string ip_file = "C://my_files//git_repo//huffman_encode_decode//ip_file.txt";
	std::string op_file = "C://my_files//git_repo//huffman_encode_decode//op_file.bin";
	encode_file.encode(ip_file, op_file);

	std::string freq_file = "C://my_files//git_repo//huffman_encode_decode//op_file.txt";
	decode_file.decode(op_file, freq_file);

	return 0;
}