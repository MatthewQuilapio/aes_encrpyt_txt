#include "aes_header.h"
#include "read_file_header.h"

int size_input = 0;

void readFile(std::string str_file_input)
{
    std::filesystem::path path_file_input = str_file_input;
    std::ifstream inFile;
    std::cout << "Path: " << path_file_input << '\n'; 
    inFile.open(str_file_input,std::ios_base::in);

    if (!inFile)
    {
        std::cerr << "Unable to open file datafile.txt";
        exit(1);   // call system to stop
    }
    else
    {
        std::string line;
        int x = 0;
        char *char_str_to_ptr;
        while (std::getline(inFile, line)) 
        {
           char *char_array_line = pad_16(line);
           array_splitter(char_array_line);

           std::cout << char_array_line << '\n'; 
        }
        
        
    }
}

void array_splitter(char *inp_array)
{
	unsigned int arr_len = sizeof(inp_array);

    switch (size_input) 
    {   
		case BYTE_16:
        {
            unsigned int uint_group = arr_len/BYTE_16;
            unsigned char array_storage[uint_group][BYTE_16];
            for(int x = 0; x < uint_group; x++)
            {
                for(int y = 0; y < BYTE_16; y++)
                {
                    array_storage[x][y] = inp_array[y];
                }
            }

            break;
        }
		case BYTE_24:
            unsigned int uint_group = arr_len/BYTE_24;
            break;
		case BYTE_32:
            unsigned int uint_group = arr_len/BYTE_32;
            break;

        default:
            std::cerr << "You input the wrong size!!";
	}
}

int main()
{
    std::string str_file_input;
    int *int_key;
    unsigned int *int_initial_byte;
    unsigned int *int_expanded_key;

    printf("Please Enter File: \n");
    std::cin >> str_file_input;

    printf("Please Enter Size: \n");
    std::cin >> size_input;

    
    int_key = get_random_bytes(BYTE_16);
    int_initial_byte = aes_initialize(sizeof(int_key));
    
    key_expansion(reinterpret_cast <unsigned int*> (int_key), reinterpret_cast <unsigned int*> (int_initial_byte));

    std::cout << int_key << '\n';  
    /* for(int i = 0; i < BYTE_24; i++)
    {
        std::cout << std::hex << int_key[i] << '\n';   
    } */
    readFile(str_file_input);
    
    return 0;
    
    exit(EXIT_SUCCESS);
   
}