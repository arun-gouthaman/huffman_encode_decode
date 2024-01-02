#include "source.h"
#include "encode.h"
#include "decode.h"


int main()
{
	Encode encode_file;
	Decode decode_file;
	
	std::string ip_file = "C://my_files//CPP//text_encode_decode//build//Debug//ip_file.txt";
	std::string op_file = "C://my_files//CPP//text_encode_decode//build//Debug//op_file.bin";
	encode_file.encode(ip_file, op_file);

	decode_file.decode("C://my_files//CPP//text_encode_decode//build//Debug//op_file.bin");

	return 0;
}