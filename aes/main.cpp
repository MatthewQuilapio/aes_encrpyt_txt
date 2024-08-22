#include "aes_header.h"
#include "read_file_header.h"

unsigned int size_input = 0;

void readFile(std::string str_file_input, unsigned char *roundKeys)
{
    std::filesystem::path path_file_input = str_file_input;
    std::vector <std::string> splitted_inp = split_string(str_file_input, '\\');
    std::string str_out_file = splitted_inp[splitted_inp.size() - 1];
    std::vector <std::string> str_file = split_string(str_out_file, '.');
    std::string str_out_filename = str_file[0];
    std::string str_final_filename = str_out_filename + "_enc.txt";
    std::ifstream inFile;
    std::ofstream outFile;
    inFile.open(str_file_input, std::ios_base::in);
    outFile.open(str_final_filename);
    

    if (!inFile)
    {
        std::cerr << "Unable to open file datafile.txt";
        exit(1);   // call system to stop
    }
    else
    {
        std::string line;
        while (std::getline(inFile, line)) 
        {
           char *char_array_line = pad_16(line);
           std::vector<std::vector<unsigned char>> splitted_array = array_splitter(char_array_line);
           unsigned char *out;
           
           for (const auto& chunk : splitted_array) 
           {
                unsigned char in[size_input + 1] = {};
                unsigned char *out = new unsigned char[size_input];
                int ctr = 0;
                for(unsigned char c: chunk)
                {
                    in[ctr] = c;
                    ctr++;
                    if (ctr == size_input)
                    {
                        in[size_input] = '\0';
                        break;
                    }  
                }

                for (unsigned int i = 0; i < size_input; i += size_input) 
                {
                    encrypt_aes(in + i, out + i, roundKeys);
                    //out_line.push_back(out[i]);
                }
                outFile << out;
           }
           outFile << '\n'; 
        }
        
    }
}

std::vector<std::string> split_string(std::string str_inp, char splitter_val)
{
    int startIndex = 0;
    int endIndex = 0;
    std::vector<std::string> output_split;
    for (int i = 0; i <= str_inp.size(); i++) 
    {
        
        // If we reached the end of the word or the end of the input.
        if (str_inp[i] == splitter_val || i == str_inp.size()) 
        {
            endIndex = i;
            std::string temp_Str;
            temp_Str.append(str_inp, startIndex, endIndex - startIndex);
            output_split.push_back(temp_Str);
            startIndex = endIndex + 1;
        }
    }
    return output_split;
}

std::vector<std::vector<unsigned char>> array_splitter(char *inp_array)
{
    unsigned int arr_len = strlen(inp_array);
    std::vector<std::vector<unsigned char>> array_storage;


    if (size_input == 16)
    {
        unsigned int uint_group = arr_len / BYTE_16;
        array_storage.resize(uint_group, std::vector<unsigned char>(BYTE_16));
        for (int x = 0; x < uint_group; x++)
        {
            for (int y = 0; y < BYTE_16; y++)
            {
                array_storage[x][y] = inp_array[(BYTE_16 * x) + y];
            }
        }
   
    }
    else if (size_input == 24)
    {
        unsigned int uint_group = arr_len / BYTE_24;
        array_storage.resize(uint_group, std::vector<unsigned char>(BYTE_24));
        for (int x = 0; x < uint_group; x++)
        {
            for (int y = 0; y < BYTE_24; y++)
            {
                array_storage[x][y] = inp_array[(BYTE_24 * x) + y];
            }
        }

    }
    else if (size_input == 32)
    {
        unsigned int uint_group = arr_len / BYTE_32;
        array_storage.resize(uint_group, std::vector<unsigned char>(BYTE_32));
        for (int x = 0; x < uint_group; x++)
        {
            for (int y = 0; y < BYTE_32; y++)
            {
                array_storage[x][y] = inp_array[(BYTE_32 * x) + y];
            }
        }

    }
    else
    {
        // Handle invalid size_input if needed
        std::cerr << "Invalid size_input value" << std::endl;
    }

    return array_storage;

}

int main()
{
    std::string str_file_input;
    //unsigned char uint_key[];
    unsigned char *uint_initial_byte;
    unsigned char *int_expanded_key;

    printf("Please Enter File: \n");
    std::cin >> str_file_input;

    printf("Please Enter Size: \n");
    std::cin >> size_input;

    
    unsigned char uint_key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    //get_random_bytes(size_input);
    aes_initialize(size_input);
    unsigned char *roundKeys = new unsigned char[4 * Nb * (Nr + 1)];
    key_expansion(uint_key, roundKeys);

    //std::cout << int_key << '\n';  
    /* for(int i = 0; i < BYTE_24; i++)
    {
        std::cout << std::hex << int_key[i] << '\n';   
    } */
    readFile(str_file_input, roundKeys);

    
    return 0;
    
    exit(EXIT_SUCCESS);
   
}