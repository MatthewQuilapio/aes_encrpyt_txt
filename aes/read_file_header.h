#include "aes_header.h"

void readFile(std::string str_file_input, unsigned char *roundKeys);
void printFormat(std::string str_file_input);
std::vector<std::vector<unsigned char>> array_splitter(char *inp_array);
std::vector<std::string> split_string(std::string str_inp, char splitter_val);

