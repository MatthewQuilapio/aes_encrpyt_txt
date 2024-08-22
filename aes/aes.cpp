#include "aes_header.h"

unsigned int Nk;
unsigned int Nr;


/*********************************************************************
 * @fn      		  - get_random_bytes
 *
 * @brief             -
 *
 * @param[in]         - number of bytes
 * @param[in]         - Pin number
 * @param[in]         -
 *
 * @return            - 
 *
 * @Note              -
 */

unsigned char* get_random_bytes(unsigned int uint_byte_size)
{
    unsigned char* intptr_return = new unsigned char[uint_byte_size];
    srand((unsigned int) time(0));   /* This is to seed the random number generator. */
    for(int i = 0; i < uint_byte_size; i++)
    {
        intptr_return[i] = rand() % 256;
        std::cout << intptr_return[i] <<" ";
    }
    printf("\n");

    return intptr_return;
}

void aes_initialize(int uint_key_size)
{
  if (uint_key_size == 16)
  {
    Nk = 4;
    Nr = 10;
  }
  else if (uint_key_size == 24)
  {
    Nk = 6;
    Nr = 12;
  }
  else if (uint_key_size == 32)
  {
    Nk = 8;
    Nr = 14;
  }
  else
  {
    exit(0);
  }
}

char* pad_16(std::string str_file_input)
{
	int original_length = str_file_input.length();
    int padding_needed = (BYTE_16 - (original_length % BYTE_16)) % BYTE_16;
    int new_length = original_length + padding_needed;
    std::stringstream str_pad;
    str_pad << std::hex << padding_needed;
    std::string hex_pad = str_pad.str();

    char* char_array_line = new char[str_file_input.length() + 1];

    for (int x = 0; x < original_length; x++)
    { 
        char_array_line[x] = str_file_input[x]; 
    } 

    for (int j = original_length; j < new_length; j++)
    {

        char_array_line[j] = hex_pad[0];
    }

    char_array_line[new_length] = '\0';

	return char_array_line;
}
/*
void pad_24(std::string str_file_input)
{
    char* char_array_line = 0;
    char_array_line = new char[str_file_input.length() + 1];
    char* char_temp_storage[16];
    for (int x = 0; x < str_file_input.length(); x++)
    { 
        char_array_line[x] = str_file_input[x]; 
    } 
        int char_array_line_modulo = 0;
        char_array_line_modulo = str_file_input.length() % BYTE_24;
        int char_arr_length = str_file_input.length();
        if (char_array_line_modulo != 0)
        {
            int append_length = 0;
            append_length = BYTE_24 - char_array_line_modulo;
            for(int j = 0; j < append_length; j++)
            {
                char_array_line[char_arr_length + j] = 'a';
            }
            char_array_line[char_arr_length + append_length] = '\0';
                //size_t array_size = sizeof(char_array_line);              
        }
        else
        {
            char_array_line[char_arr_length] = '\0';
        }
        std::cout << char_array_line << '\n';
}

void pad_32(std::string str_file_input)
{
    char* char_array_line = 0;
    char_array_line = new char[str_file_input.length() + 1];
    char* char_temp_storage[16];
    for (int x = 0; x < str_file_input.length(); x++)
    { 
        char_array_line[x] = str_file_input[x]; 
    } 
        int char_array_line_modulo = 0;
        char_array_line_modulo = str_file_input.length() % BYTE_32;
        int char_arr_length = str_file_input.length();
        if (char_array_line_modulo != 0)
        {
            int append_length = 0;
            append_length = BYTE_32 - char_array_line_modulo;
            for(int j = 0; j < append_length; j++)
            {
                char_array_line[char_arr_length + j] = 'a';
            }
            char_array_line[char_arr_length + append_length] = '\0';
                //size_t array_size = sizeof(char_array_line);              
        }
        else
        {
            char_array_line[char_arr_length] = '\0';
        }
        std::cout << char_array_line << '\n';

}*/

void key_expansion(unsigned char *uint_key, unsigned char *uint_init_val)
{
  unsigned char temp[4];
  unsigned char rcon[4];

  unsigned int i = 0;
  while (i < 4 * Nk)
  {
    uint_init_val[i] = uint_key[i];
    i++;
  }

  i = 4 * Nk;
  while (i < 4 * Nb * (Nr + 1)) 
  {
    temp[0] = uint_init_val[i - 4 + 0];
    temp[1] = uint_init_val[i - 4 + 1];
    temp[2] = uint_init_val[i - 4 + 2];
    temp[3] = uint_init_val[i - 4 + 3];

    if (i / 4 % Nk == 0) 
    {
      RotWord(temp);
      SubWord(temp);
      Rcon(rcon, i / (Nk * 4));
      Xor_Words(temp, rcon, temp);
    } else if (Nk > 6 && i / 4 % Nk == 4) 
    {
      SubWord(temp);
    }

    uint_init_val[i + 0] = uint_init_val[i - 4 * Nk] ^ temp[0];
    uint_init_val[i + 1] = uint_init_val[i + 1 - 4 * Nk] ^ temp[1];
    uint_init_val[i + 2] = uint_init_val[i + 2 - 4 * Nk] ^ temp[2];
    uint_init_val[i + 3] = uint_init_val[i + 3 - 4 * Nk] ^ temp[3];
    i += 4;
  }    
}
void RotWord(unsigned char *a) 
{
  unsigned char c = a[0];
  a[0] = a[1];
  a[1] = a[2];
  a[2] = a[3];
  a[3] = c;
}

void XorWords(unsigned char *a, unsigned char *b, unsigned char *c) 
{
  int i;
  for (i = 0; i < 4; i++) 
  {
    c[i] = a[i] ^ b[i];
  }
}

void Rcon(unsigned char *a, unsigned int n) 
{
  unsigned int i;
  unsigned char c = 1;
  for (i = 0; i < n - 1; i++) 
  {
    c = xtime(c);
  }

  a[0] = c;
  a[1] = a[2] = a[3] = 0;
}

unsigned char xtime(unsigned char b)  // multiply on x
{
  return (b << 1) ^ (((b >> 7) & 1) * 0x1b);
}

void SubWord(unsigned char *a) 
{
  int i;
  for (i = 0; i < 4; i++) {
    a[i] = sbox[a[i] / 16][a[i] % 16];
  }
}

void encrypt_aes(const unsigned char in[], unsigned char out[], unsigned char *roundKeys) 
{
  unsigned char state[4][Nb];
  unsigned int i, j, round;

  for (i = 0; i < 4; i++) 
  {
    for (j = 0; j < Nb; j++) 
    {
      state[i][j] = in[i + 4 * j];
    }
  }

  Add_Round_Key(state, roundKeys);

  for (round = 1; round <= Nr - 1; round++) {
    Sub_Bytes(state);
    Shift_Rows(state);
    Mix_Columns(state);
    Add_Round_Key(state, roundKeys + round * 4 * Nb);
  }

  Sub_Bytes(state);
  Shift_Rows(state);
  Add_Round_Key(state, roundKeys + Nr * 4 * Nb);

  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      out[i + 4 * j] = state[i][j];
    }
  }
}

void Sub_Bytes(unsigned char state[4][Nb])
{
  unsigned int i, j;
  unsigned char t;
  for (i = 0; i < 4; i++) 
  {
    for (j = 0; j < Nb; j++)
    {
      t = state[i][j];
      state[i][j] = sbox[t / 16][t % 16];
    }
  }
}

void Shift_Rows(unsigned char state[4][Nb]) 
{
  func_row(state, 1, 1);
  func_row(state, 2, 2);
  func_row(state, 3, 3);
}


void func_row(unsigned char state[4][Nb], unsigned int i, unsigned int n)  // shift row i on n positions
{
  unsigned char tmp[Nb];
  for (unsigned int j = 0; j < Nb; j++) 
  {
    tmp[j] = state[i][(j + n) % Nb];
  }
  memcpy(state[i], tmp, Nb * sizeof(unsigned char));
}

void Add_Round_Key(unsigned char state[4][Nb], unsigned char *key) {
  unsigned int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < Nb; j++) {
      state[i][j] = state[i][j] ^ key[i + 4 * j];
    }
  }
}

void Mix_Columns(unsigned char state[4][Nb]) {
  unsigned char temp_state[4][Nb];

  for (size_t i = 0; i < 4; ++i) {
    memset(temp_state[i], 0, 4);
  }

  for (size_t i = 0; i < 4; ++i) {
    for (size_t k = 0; k < 4; ++k) {
      for (size_t j = 0; j < 4; ++j) {
        if (CMDS[i][k] == 1)
          temp_state[i][j] ^= state[k][j];
        else
          temp_state[i][j] ^= GF_MUL_TABLE[CMDS[i][k]][state[k][j]];
      }
    }
  }
}

void Xor_Words(unsigned char *a, unsigned char *b, unsigned char *c) {
  int i;
  for (i = 0; i < 4; i++) {
    c[i] = a[i] ^ b[i];
  }
}


