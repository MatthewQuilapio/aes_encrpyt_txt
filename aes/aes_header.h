#ifndef aes_header
#define aes_header

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <iomanip>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h> 

#define BYTE_16     16
#define BYTE_24     24
#define BYTE_32     32

void Add_Round_Key();
void Sub_Bytes();
void Shift_Rows();
void Mix_Columns();
void AES_Cipher(uint8_t *in, uint8_t *out, uint8_t *w);
unsigned int *aes_initialize(size_t uint_key_size);
unsigned int *key_expansion(unsigned int *uint_key, unsigned int *uint_init_val);
char* pad_16(std::string str_file_input);
void pad_24(std::string str_file_input);
void pad_32(std::string str_file_input);
int* get_random_bytes(unsigned int uint_byte_size);
int *get_key_size(unsigned int uint_key_size);

extern    int int_no_keys, int_no_round;
extern    int int_no_column = 4;



#endif 