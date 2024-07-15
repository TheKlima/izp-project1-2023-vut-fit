#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_ADDRESS_STR_LENGTH 100
#define MAX_ADDRESS_BUFFER_LENGTH (MAX_ADDRESS_STR_LENGTH + 1) // + '\0'

#define MAX_INPUT_ADDRESSES_COUNT 1000

#define ASCII_TABLE_SIZE 128
#define NON_PRINTABLE_ASCII_CHARS_COUNT 32

void strToUpper(char* str);

bool isValidArgCount(int arg_count)
{
    return arg_count <= 1;
}

bool isValidArgLength(char* arg)
{
    return strlen(arg) <= MAX_ADDRESS_STR_LENGTH;
}

bool isValidArg(int arg_count, char* arg)
{
    return isValidArgCount(arg_count) && (arg_count == 0 || isValidArgLength(arg));
}

void getSearchedAddress(int arg_count, char** argv, char* searched_address)
{
    if(arg_count != 0)
    {
        strcpy(searched_address, argv[1]);
        strToUpper(searched_address);
    }
}

bool addressContainsAlphaChar(char* address, int address_str_length)
{
    for(int i = 0; i < address_str_length; ++i)
    {
        if(isalpha(address[i]))
        {
            return true;
        }
    }

    return false;
}

bool isValidInputAddress(char* address)
{
    int address_str_length = strlen(address);
    
    if(!addressContainsAlphaChar(address, address_str_length) || (address_str_length == 101 && address[address_str_length - 1] != '\n'))
    {
        return false;
    }
    
    address[address_str_length - 1] = '\0';
    return true;
}

void strToUpper(char* str)
{
    for(int i = 0; str[i] != '\0'; ++i)
    {
        str[i] = toupper(str[i]);
    }
}

bool parseInputAddresses(Addresses_database* addresses_database)
{
    while(fgets(addresses_database->input_addresses[addresses_database->input_addresses_count], MAX_ADDRESS_BUFFER_LENGTH + 1, stdin) != NULL)
    {
        if(addresses_database->input_addresses_count == MAX_INPUT_ADDRESSES_COUNT ||
           !isValidInputAddress(addresses_database->input_addresses[addresses_database->input_addresses_count]))
        {
            return false;
        }

        strToUpper(addresses_database->input_addresses[addresses_database->input_addresses_count]);
        ++(addresses_database->input_addresses_count);
    }
    
    return true;
}

bool isFullyMatchedAddress(char* address_from_database, int searched_address_str_length)
{
    return address_from_database[searched_address_str_length - 1] != '\0' &&
           address_from_database[searched_address_str_length] == '\0';
}

void printPossibleChars(bool* possible_chars)
{
    printf("Enable: ");
    for(int i = 0; i < ASCII_TABLE_SIZE - NON_PRINTABLE_ASCII_CHARS_COUNT; ++i)
    {
        if(possible_chars[i])
        {
            putchar(i + NON_PRINTABLE_ASCII_CHARS_COUNT);
        }
    }

    putchar('\n');
}

void runVirtualKeyboard(char* searched_address, Addresses_database* addresses_database)
{
    bool possible_chars[ASCII_TABLE_SIZE - NON_PRINTABLE_ASCII_CHARS_COUNT] = {false, };
    
    int first_matched_address_idx = -1;
    int matched_addresses_count = 0; 
    
    int searched_address_str_length = strlen(searched_address);
    for(int i = 0; i < addresses_database->input_addresses_count; ++i)
    {
        if(strncmp(searched_address, addresses_database->input_addresses[i], searched_address_str_length) == 0)
        {
            if(matched_addresses_count == 0)
            {
                first_matched_address_idx = i;
            }
            
            if(!isFullyMatchedAddress(addresses_database->input_addresses[i], searched_address_str_length))
            {
                possible_chars[addresses_database->input_addresses[i][searched_address_str_length] - NON_PRINTABLE_ASCII_CHARS_COUNT] = true;
            }
            else
            {
                printf("Found: %s\n", searched_address);
            }
            
            ++matched_addresses_count;
        }
    }
    
    if(matched_addresses_count > 1)
    {
        printPossibleChars(possible_chars);
    }
    else if(matched_addresses_count == 1)
    {
        if(!isFullyMatchedAddress(addresses_database->input_addresses[first_matched_address_idx], searched_address_str_length))
        {
            printf("Found: %s\n", addresses_database->input_addresses[first_matched_address_idx]);
        }
    }
    else
    {
        printf("Not found\n");
    }
}

bool runVirtualKeyboard(char* searched_address)
{
}

int main(int argc, char** argv)
{
    if(!isValidArg(argc - 1, argv[1]))
    {
        fprintf(stderr, "Error! Wrong number of program arguments (expected 0 or 1) or invalid length of first argument.\n");
        return EXIT_FAILURE;
    }

    char searched_address[MAX_ADDRESS_BUFFER_LENGTH] = {'\0', };
    getSearchedAddress(argc - 1, argv, searched_address);
    
    if(!runVirtualKeyboard(searched_address))
    {
        fprintf(stderr, "Error! Too many input addresses provided or invalid input address provided.\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}