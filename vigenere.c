#include <stdlib.h>
#include <string.h>
#include <math.h>
//===========================================================================================================
#include "vigenere.h"
//===========================================================================================================
const int ALPHABET_POWER = 26;
const int MAX_KEY_SIZE = 12;
const int SIZE_ARRAY_NUMBERS_OF_CHARACTERS = 256;
char * p_alphabet;
char ** p_vigenere_square;
unsigned int * p_array_numbers_of_character;
char * p_key;
char * p_guess_key;
int key_size;
//===========================================================================================================
double * pd_symbols_frequence_part_of_one;
double ** pd_symbols_frequence_into_crypted_text;
double * pd_convolution_buffer;
//===========================================================================================================
/// @brief set_data function allocate memory for variables and initialize with constant values
/// @return 0 if success, negative value if an error occurred
int set_data()
{
    int i_to_return = 0;
    int row_count = 0;
    int mod_count = 0;
    p_alphabet = (char*)malloc(ALPHABET_POWER*sizeof(char));
    if (p_alphabet == NULL) {
        i_to_return = -1;
        return i_to_return;
    }
    pd_symbols_frequence_part_of_one = (double*)malloc(ALPHABET_POWER*sizeof(double));
    if (pd_symbols_frequence_part_of_one  == NULL) {
        i_to_return = -1;
        return i_to_return;
    }
    pd_convolution_buffer = (double*)malloc(ALPHABET_POWER*sizeof(double));
    if (pd_convolution_buffer == NULL) {
        i_to_return = -1;
        return i_to_return;
    }
    p_array_numbers_of_character = (unsigned int*)malloc(SIZE_ARRAY_NUMBERS_OF_CHARACTERS*sizeof(int));
    if (p_array_numbers_of_character == NULL) {
        i_to_return = -1;
        return i_to_return;
    }


    p_guess_key = (char*)malloc((MAX_KEY_SIZE+1)*sizeof(char));
    if (p_guess_key == NULL) {
        i_to_return = -1;
        return i_to_return;
    }
    p_key = (char*)malloc((MAX_KEY_SIZE+1)*sizeof(char));
    if (p_key == NULL) {
        i_to_return = -1;
        return i_to_return;
    }
    p_vigenere_square = (char**)malloc(ALPHABET_POWER*sizeof(char*));
    if (p_vigenere_square == NULL) {
        i_to_return = -1;
        return i_to_return;
    }
    for(row_count = 0; row_count < ALPHABET_POWER; ++row_count) {
        p_vigenere_square[row_count] = (char*)malloc(ALPHABET_POWER*sizeof(char));
        if (p_vigenere_square[row_count] == NULL) {
            i_to_return = -1;
            return i_to_return;
        }
    }
    pd_symbols_frequence_into_crypted_text = (double**)malloc((MAX_KEY_SIZE+1)*sizeof(double*));
    if (pd_symbols_frequence_into_crypted_text == NULL) {
        i_to_return = -1;
        return i_to_return;
    }
    for (mod_count = 0; mod_count <= MAX_KEY_SIZE; ++mod_count) {
        pd_symbols_frequence_into_crypted_text[mod_count] = (double*)malloc(ALPHABET_POWER*sizeof(double));
        if (pd_symbols_frequence_into_crypted_text[mod_count] == NULL) {
            i_to_return = -1;
            return i_to_return;
        }
    }


    set_english_lower_character_alphabet(p_alphabet);
    set_english_symbols_typical_frequence(pd_symbols_frequence_part_of_one);
    set_vigenere_square(p_vigenere_square, p_alphabet, ALPHABET_POWER);
    set_array_numbers_of_charasters(p_array_numbers_of_character, p_alphabet, ALPHABET_POWER);

    return i_to_return;
}
//===========================================================================================================
/// @brief freeing previously allocated memory
void free_data()
{
    int row_count = 0;
    int mod_count = 0;

    free(p_alphabet);
    free(pd_symbols_frequence_part_of_one);
    free(pd_convolution_buffer);
    free(p_array_numbers_of_character);
    free(p_guess_key);
    free(p_key);
    for(row_count = 0; row_count < ALPHABET_POWER; ++row_count) {
        free(p_vigenere_square[row_count]);
    }
    free(p_vigenere_square);
    for (mod_count = 0; mod_count <= MAX_KEY_SIZE; ++mod_count) {
        free(pd_symbols_frequence_into_crypted_text[mod_count]);
    }
    free(pd_symbols_frequence_into_crypted_text);
}
//===========================================================================================================
/// @brief vigenere_encrypt function encrypt source file into destination file with vigenere cipher
/// @param p_source_file name of source file
/// @param p_dest_file name of destination file
/// @return 0 if success, negative value if an error occurred
int vigenere_encrypt(const char * p_source_file, const char * p_dest_file)
{
    int i_to_return = 0, position=0;
    char ch_from_file;
    FILE * p_file_source = fopen(p_source_file, "r");
    if (p_file_source == NULL) {
        perror("cannot open source file");
        i_to_return = -1;
    }
    else {
        FILE * p_file_dest = fopen(p_dest_file, "w");
        if (p_file_dest == NULL) {
            perror("cannot open dest file");
            if(fclose(p_file_source)) {
                perror("error at closing  source file");
                i_to_return = -1;
            }
            else {
                i_to_return = -1;
            }
        }
        else {
            while((ch_from_file = fgetc(p_file_source)) != EOF) {
                //-------------------------
                //it strange after trying in windows os
                if (!((int)ch_from_file >= -1 && (int)ch_from_file < 255)) {
                    continue;
                }
                //-------------------------
                if(isalpha((int)ch_from_file)) {
                    ch_from_file = get_vigenere_encrypted_char(ch_from_file, position, p_key, key_size);
                    position++;
                }
                if(EOF == putc(ch_from_file, p_file_dest)) {
                    perror("error at writing dest file");
                    i_to_return = -1;
                    break;
                }
            }
            if (fclose(p_file_dest)) {
                perror("error at closing dest file");
                i_to_return = -1;
            }
            if(fclose(p_file_source)) {
                perror("error at closing  source file");
                i_to_return = -1;
            }
            if (!position) {
                fprintf(stderr, "no symols to encrypt in source file\n");
                if(remove(p_dest_file)) {
                    perror("cannot remove dest file");
                }
                i_to_return = -1;
            }
        }
    }
    return i_to_return;
}
//===========================================================================================================
/// @brief vigenere_decrypt function decrypt source file into destination file with vigenere cipher
/// @param p_source_file name of source file
/// @param p_dest_file name of destination file
/// @return 0 if success, negative value if an error occurred
int vigenere_decrypt(const char * p_source_file, const char * p_dest_file)
{
    int i_to_return = 0, position=0;
    char ch_from_file;
    FILE * p_file_dest = NULL, * p_file_source = NULL;
    p_file_source = fopen(p_source_file, "r");
    if (p_file_source == NULL) {
        perror("cannot open source file");
        i_to_return = -1;
    }
    else {
        p_file_dest = fopen(p_dest_file, "w");
        if (p_file_dest == NULL) {
            perror("cannot open dest file");
            if(fclose(p_file_source)) {
                perror("error at closing  source file");
                i_to_return = -1;
            }
            else {
                i_to_return = -1;
            }
        }
        else {
            while((ch_from_file = fgetc(p_file_source)) != EOF) {
                if(isalpha(ch_from_file)) {
                    ch_from_file = get_vigenere_decrypted_char(ch_from_file, position, p_key, key_size);
                    position++;
                }
                if(EOF == putc(ch_from_file, p_file_dest)) {
                    perror("error at writing dest file");
                    i_to_return = -1;
                    break;
                }
            }
            if (fclose(p_file_dest)) {
                perror("error at closing dest file");
                i_to_return = -1;
            }
            if(fclose(p_file_source)) {
                perror("error at closing  source file");
                i_to_return = -1;
            }
            if (!position) {
                fprintf(stderr, "no symols to decrypt in source file\n");
                if(remove(p_dest_file)) {
                    perror("cannot remove dest file");
                }
                i_to_return = -1;
            }
        }
    }
    return i_to_return;
}
//===========================================================================================================
/// @brief vigenere_decrypt_with_guess_key decrypt file passed to argument list as input file stream
/// used p_guess_key as a guess_key, and put result plain test into console output
/// @param p_file_source an input file stream, that previously was open
/// @return 0 if success, , negative value if an error occurred
int vigenere_decrypt_with_guess_key(FILE * p_file_source, int lenght_guess_key)
{
    int i_to_return = 0, position = 0;
    char ch_from_file;
    if(fseek(p_file_source, 0, SEEK_SET)) {
        i_to_return = -1;
    }
    else {
        printf("decryptes with guess key text:\n");
        while((ch_from_file = fgetc(p_file_source)) != EOF) {
            if(isalpha(ch_from_file)) {
                ch_from_file = get_vigenere_decrypted_char(ch_from_file, position, p_guess_key, lenght_guess_key);
                position++;
            }
            printf("%c", ch_from_file);
        }
    }
    return i_to_return;
}
//===========================================================================================================
/// @brief set subarrays with decimation step (mod) from 0 to MAX_KEY_SIZE
/// @post in two dimension array 'pd_symbols_frequence_into_crypted_text' stored normalize frequencies of
/// symbols of text with decimation from 0 to MAX_KEY_SIZE
/// @param p_file_source input file stream (it should be at begin file)
/// @return 0 if success, negative value if an error occurred
int set_subarrays_symbols_frequence_into_crypted_text(FILE * p_file_source)
{
    int i_to_return = 0, symbols_count = 0, letter_count = 0, mod = 1, i_alphabet_char_index;
    double divider = 1.0;
    char ch_from_file;

    while((ch_from_file = fgetc(p_file_source)) != EOF) {
        if(isalpha(ch_from_file)) {
            ch_from_file = tolower((int)ch_from_file);
            i_alphabet_char_index = p_array_numbers_of_character[(unsigned int)ch_from_file];
            for(mod = 1; mod <= MAX_KEY_SIZE+1; ++mod) {
                if (symbols_count % mod == 0) {
                    pd_symbols_frequence_into_crypted_text[mod-1][i_alphabet_char_index] += 1.0;
                }
            }
            symbols_count++;
        }
    }
    if (!symbols_count) {
        fprintf(stderr, "no symbols in source file\n");
        i_to_return = -1;
        return i_to_return;
    }

    for (mod = 0; mod <= MAX_KEY_SIZE; ++mod) {
        divider = (double)symbols_count/(double)(mod+1);
        for (letter_count = 0; letter_count < ALPHABET_POWER; ++letter_count) {
            pd_symbols_frequence_into_crypted_text[mod][letter_count] /= divider;
        }
    }
    return i_to_return;
}
//===========================================================================================================
/// @brief set subarrays with decimation step (mod) from 0 to MAX_KEY_SIZE
/// @post in two dimension array 'pd_symbols_frequence_into_crypted_text' stored normalize frequencies of
/// symbols of text with decimation from 0 to MAX_KEY_SIZE
/// @param pch_encrypted_text_array input encrypted text in char array
/// @param lenght_encrypted_text_array lenght of the array
/// @return 0 if success, negative value if an error occurred
int set_subarrays_symbols_from_char_array(char * pch_encrypted_text_array, const int lenght_encrypted_text_array)
{
    int i_to_return = 0, position = 0, symbol_count = 0, mod = 0, letter_count = 0, i_alphabet_char_index;
    double divider = 1.0;
    char ch_from_array;
    for(position = 0; position < lenght_encrypted_text_array; ++position) {
        ch_from_array = pch_encrypted_text_array[position];
        if (isalpha(ch_from_array)) {
            ch_from_array = tolower((int)ch_from_array);
            i_alphabet_char_index = p_array_numbers_of_character[(unsigned int)ch_from_array];
            for(mod = 1; mod <= MAX_KEY_SIZE+1; ++mod) {
                if (symbol_count % mod == 0) {
                    pd_symbols_frequence_into_crypted_text[mod-1][i_alphabet_char_index] += 1.0;
                }
            }
            symbol_count++;
        }
    }
    if (!symbol_count) {
        fprintf(stderr, "no symbols in source file\n");
        i_to_return = -1;
        return i_to_return;
    }
    for (mod = 0; mod <= MAX_KEY_SIZE; ++mod) {
        divider = (double)symbol_count/(double)(mod+1);
        for (letter_count = 0; letter_count < ALPHABET_POWER; ++letter_count) {
            pd_symbols_frequence_into_crypted_text[mod][letter_count] /= divider;
        }
    }
    return i_to_return;
}
//===========================================================================================================
/// @brief set_subarrays_symbols_frequence_guess_key is simulary to
/// set_subarrays_symbols_frequence_into_crypted_text, but when we call it we know guess of lenght key
/// @param p_file_source input file stream (it should be at begin file)
/// @param ppd_array_frequencies two dimension array (guess_lenght_key x ALPHABET_POWER)
/// function will stored and normalize frequencies of symbols into the array
/// @param lenght_guess_key guess lenght of vigenere cipher key
/// @return 0 if success, negative value if an error occured
int set_subarrays_symbols_frequence_guess_key(FILE * p_file_source,
                                              double ** ppd_array_frequencies,
                                              int lenght_guess_key)
{
    int i_to_return = 0, symbols_count = 0, letter_count = 0, key_symbol_count = 0,
        rest_from_guess_key, i_alphabet_char_index;
    double divider;
    char ch_from_file;
    while((ch_from_file = fgetc(p_file_source)) != EOF) {
        if(isalpha(ch_from_file)) {
            ch_from_file = tolower((int)ch_from_file);
            i_alphabet_char_index = p_array_numbers_of_character[(unsigned int)ch_from_file];
            rest_from_guess_key = (symbols_count -1) % lenght_guess_key;

            if (rest_from_guess_key >= 0 && (rest_from_guess_key != (lenght_guess_key-1))) {
                ppd_array_frequencies[rest_from_guess_key][i_alphabet_char_index] += 1.0;
            }
            symbols_count++;
        }
    }

    divider = (double)symbols_count/(double)(lenght_guess_key);
    for (key_symbol_count = 0; key_symbol_count < (lenght_guess_key-1); ++key_symbol_count) {
        for (letter_count = 0; letter_count < ALPHABET_POWER; ++letter_count) {
            ppd_array_frequencies[key_symbol_count][letter_count] /= divider;
        }
    }
    if (symbols_count == 0) {
        i_to_return = -1;
        fprintf(stderr, "no symbol find in input p_file_source");
    }
    return i_to_return;
}
//===========================================================================================================
/// @brief set_subarrays_symbols_guess_key_from_char_array is simulary to
/// set_subarrays_symbols_frequence_guess_key, but when we call it we get data from char array
/// @param pch_encrypted_text_array input char array of crypted text
/// @param lenght_encrypted_text_array the lenght of the input array
/// @param ppd_array_frequencies two dimension array (guess_lenght_key x ALPHABET_POWER)
/// function will stored and normalize frequencies of symbols into the array
/// @param lenght_guess_key guess lenght of vigenere cipher key
/// @return 0 if success, negative value if an error occured
int set_subarrays_symbols_guess_key_from_char_array(char * pch_encrypted_text_array,
                                                    const int lenght_encrypted_text_array,
                                                    double ** ppd_array_frequencies,
                                                    int lenght_guess_key)
{
    int i_to_return = 0, symbol_count = 0, letter_count = 0, key_symbol_count = 0, position = 0,
        rest_from_guess_key, i_alphabet_char_index;
    double divider;
    char ch_from_array;

    for(position = 0; position < lenght_encrypted_text_array; ++position) {
        ch_from_array = pch_encrypted_text_array[position];
        if (isalpha(ch_from_array)) {
            ch_from_array = tolower((int)ch_from_array);
            i_alphabet_char_index = p_array_numbers_of_character[(unsigned int)ch_from_array];

            rest_from_guess_key = (symbol_count -1) % lenght_guess_key;

            if (rest_from_guess_key >= 0 && (rest_from_guess_key != (lenght_guess_key-1))) {
                ppd_array_frequencies[rest_from_guess_key][i_alphabet_char_index] += 1.0;
            }

            symbol_count++;
        }
    }

    divider = (double)symbol_count/(double)(lenght_guess_key);
    for (key_symbol_count = 0; key_symbol_count < (lenght_guess_key-1); ++key_symbol_count) {
        for (letter_count = 0; letter_count < ALPHABET_POWER; ++letter_count) {
            ppd_array_frequencies[key_symbol_count][letter_count] /= divider;
        }
    }
    if (symbol_count == 0) {
        i_to_return = -1;
        fprintf(stderr, "no symbol find in input p_file_source");
    }
    return i_to_return;
}
//===========================================================================================================
/// @brief convolution_research it function do correclation arrays of frequencies of symbols with typical
/// frequencies in english texts, and store result into two dimension array 'pd_symbols_frequence_into_crypted_text'
/// after that find in the arrays index of array (p_index_mod_with_maximum) with maximum ratio (maximum : middle level)
/// the value of the index is guess lenght of vigenere cipher key, and symbol related to
/// index maximum (index_key_symbol) is first symbol of  guess vigenere cipher key
/// @param [out] p_index_mod_with_maximum pointer to value of guess lenght of vigenere cipher key
/// will be stored into the function
/// @param [out] p_index_key_symbol pointer to value of index first letter in the key
/// @return 0 if success, negative value if an error occurred
int convolution_research(int * p_index_mod_with_maximum, int * p_index_key_symbol)
{
    int i_to_return = 0, mod_count = 0;
    double array_max_to_middle_others_relation[MAX_KEY_SIZE+1];
    double array_sigma_middle_level[MAX_KEY_SIZE+1];
    int array_index_maximum[MAX_KEY_SIZE+1];

    double maximum_in_modes = -1.0;
    for (mod_count = 0; mod_count <= MAX_KEY_SIZE; ++mod_count) {
        convolution_with_basic_sequence(pd_symbols_frequence_into_crypted_text[mod_count],
                                        pd_symbols_frequence_part_of_one,
                                        pd_convolution_buffer,
                                        ALPHABET_POWER);
    }


    for (mod_count = 0; mod_count < MAX_KEY_SIZE; ++mod_count) {
        correlation_research(pd_symbols_frequence_into_crypted_text[mod_count],
                             ALPHABET_POWER,
                             &array_max_to_middle_others_relation[mod_count],
                             &array_sigma_middle_level[mod_count],
                             &array_index_maximum[mod_count]);
        if (maximum_in_modes <  array_max_to_middle_others_relation[mod_count]) {
            maximum_in_modes = array_max_to_middle_others_relation[mod_count];
            *p_index_mod_with_maximum = mod_count;
            *p_index_key_symbol = array_index_maximum[mod_count];
        }
    }
    if (maximum_in_modes < 0) {
        i_to_return = -1;
    }
    return i_to_return;
}
//===========================================================================================================
/// @brief convolution_research_rested_key_symbols it function like convolution_research
/// do correlation arrays of frequencies of symbols with typical
/// frequencies in english texts, and store result into two dimension array 'ppd_array_frequencies'
/// passed as argument, result of correlation is maximums into subarrays, each index of maximum
/// is represent number of symbol in alphabet therefore we can guess key of vigenere cipher
/// @param ppd_array_frequencies
int convolution_research_rested_key_symbols(double ** ppd_array_frequencies, int lenght_guess_key)
{
    int i_to_return = 0;
    double max_to_middle_others_relation;
    double sigma_middle_level;
    int index_maximum, key_symbol_count;
    for (key_symbol_count = 0; key_symbol_count < lenght_guess_key - 1; ++key_symbol_count) {
        convolution_with_basic_sequence(ppd_array_frequencies[key_symbol_count],
                                        pd_symbols_frequence_part_of_one,
                                        pd_convolution_buffer,
                                        ALPHABET_POWER);
        correlation_research(ppd_array_frequencies[key_symbol_count],
                             ALPHABET_POWER,
                             &max_to_middle_others_relation,
                             &sigma_middle_level,
                             &index_maximum);
        p_guess_key[key_symbol_count+1] = p_alphabet[index_maximum];
    }
    return i_to_return;
}
//===========================================================================================================
/// @brief allocate two dimension array for search rest symbols of key
/// @param pd_symbols_frequence_guess_key it will be two dimension array for
/// frequence decimate symbols
/// @return pointer to
double ** set_key_search_data(int lenght_guess_key)
{
    int key_symbol_count = 0;
    int alphabet_symbol_count = 0;
    double ** pd_symbols_frequence_guess_key = (double**)malloc((lenght_guess_key-1)*sizeof(double*));
    if (pd_symbols_frequence_guess_key == NULL) {
        perror("malloc failed");
        return NULL;
    }
    for (key_symbol_count = 0; key_symbol_count < (lenght_guess_key-1); ++key_symbol_count) {
        pd_symbols_frequence_guess_key[key_symbol_count] = (double*)malloc(ALPHABET_POWER*sizeof(double));
        if (pd_symbols_frequence_guess_key[key_symbol_count] == NULL) {
            perror("malloc failed");
            return NULL;
        }
    }
    for (key_symbol_count = 0; key_symbol_count < (lenght_guess_key-1); ++key_symbol_count) {
        for (alphabet_symbol_count = 0; alphabet_symbol_count < ALPHABET_POWER; ++alphabet_symbol_count) {
            pd_symbols_frequence_guess_key[key_symbol_count][alphabet_symbol_count] = 0.0;
        }
    }
    return pd_symbols_frequence_guess_key;
}
//===========================================================================================================
/// @brief freeing data for search symbols of guess key
/// @param pp_data pointer to data will be freed
/// @param lenght_guess_key lenght of guess key aka dimension of two dimension array
void free_key_search_data(double ** pp_data, int lenght_guess_key)
{
    int key_symbol_count = 0;
    for (key_symbol_count = 0; key_symbol_count < (lenght_guess_key-1); ++key_symbol_count) {
        free(pp_data[key_symbol_count]);
    }
    free(pp_data);
}
//===========================================================================================================
/// @brief reset_array_symbols_frequence_into_crypted_text will reset values in array of frequencies
/// of symbols to 0
void reset_array_symbols_frequence_into_crypted_text()
{
    int mod_count, symbol_count;
    for (mod_count = 0; mod_count <= MAX_KEY_SIZE; ++mod_count) {
        for (symbol_count = 0; symbol_count < ALPHABET_POWER; ++symbol_count) {
            pd_symbols_frequence_into_crypted_text[mod_count][symbol_count] = 0.0;
        }
    }
}
//===========================================================================================================
/// @brief frequence_analiz do frequence analyz symbols in text with decimation, it may find guessed lenght
/// of key, symbols of key and do decrypting the source file
/// @param p_source_file name of source file
/// @return 0 if success, negative value if an error occurred
int frequence_analiz(const char * p_source_file)
{
    int i_to_return = 0;
    FILE * p_file_source = NULL;
    int index_mod_with_maximum, index_key_symbol;
    int lenght_guess_key;
    double ** pd_symbols_frequence_guess_key = NULL;
    reset_array_symbols_frequence_into_crypted_text();
    p_file_source = fopen(p_source_file, "r");
    if (p_file_source == NULL) {
        perror("cannot open file");
        i_to_return = -1;
        return i_to_return;
    }
    else {
        if(set_subarrays_symbols_frequence_into_crypted_text(p_file_source) < 0) {
            fprintf(stderr,"set_subarrays_symbols_frequence_into_crypted_text failed\n");
            i_to_return = -1;
            return i_to_return;
        }


        if(convolution_research(&index_mod_with_maximum, &index_key_symbol) < 0) {
            fprintf(stderr,"convolution_research failed");
        }

        lenght_guess_key = index_mod_with_maximum + 1;
        if (lenght_guess_key > MAX_KEY_SIZE) {
            fprintf(stderr, "lenght_guess_key: %d, greater than MAX_KEY_SIZE: %d\n", lenght_guess_key, MAX_KEY_SIZE);
        }
        p_guess_key[0] = p_alphabet[index_key_symbol];
        p_guess_key[lenght_guess_key] = '\0';
        if (lenght_guess_key != 1) {
            if(fseek(p_file_source, 0, SEEK_SET)) {
                perror("fseek failed");
                i_to_return = -1;
                return i_to_return;
            }
            pd_symbols_frequence_guess_key = set_key_search_data(lenght_guess_key);
            if (pd_symbols_frequence_guess_key == NULL) {
                fprintf(stderr, "set_key_search_data failed");
                i_to_return = -1;
                return i_to_return;
            }
            if(set_subarrays_symbols_frequence_guess_key(p_file_source,
                                                         pd_symbols_frequence_guess_key,
                                                         lenght_guess_key) < 0) {
                fprintf(stderr, "set_subarrays_symbols_frequence_guess_key failed");
                if (pd_symbols_frequence_guess_key != NULL) {
                    free_key_search_data(pd_symbols_frequence_guess_key, lenght_guess_key);
                }
                i_to_return = -1;
                return i_to_return;
            }
            if (convolution_research_rested_key_symbols(pd_symbols_frequence_guess_key, lenght_guess_key) < 0) {
                fprintf(stderr, "convolution_research_rested_key_symbols failed");
                if (pd_symbols_frequence_guess_key != NULL) {
                    free_key_search_data(pd_symbols_frequence_guess_key, lenght_guess_key);
                }
                i_to_return = -1;
                return i_to_return;
            }
            if (pd_symbols_frequence_guess_key != NULL) {
                free_key_search_data(pd_symbols_frequence_guess_key, lenght_guess_key);
            }
        }
        if (vigenere_decrypt_with_guess_key(p_file_source, lenght_guess_key) < 0) {
            fprintf(stderr, "vigenere_decrypt_with_guess_key failed");
            i_to_return = -1;
        }
        else {
            printf("\nthe guess key is \'%s\' \n", p_guess_key);
        }
        if(fclose(p_file_source)) {
            perror("error at closing file");
            i_to_return = -1;
        }
    }
    return i_to_return;
}
//===========================================================================================================
/// @brief set_vigenere_square store vigenere square passed as argument 'p_vigenere_square'
/// @param [out] p_vigenere_square two dimension array for store vigenere square
/// @param p_alphabet pointer to alphabet
/// @param alphabet_power quantity letters in the alpabet
void set_vigenere_square(char ** p_vigenere_square, char * p_alphabet, const int alphabet_power)
{
    int row_count, column_count;
    for(row_count = 0; row_count < alphabet_power; ++row_count) {
        for(column_count = 0; column_count < alphabet_power; ++column_count) {
            p_vigenere_square[row_count][column_count] = p_alphabet[(column_count + row_count) % alphabet_power];
        }
    }
}
//===========================================================================================================
/// @brief set array of english aplphabet in direct order of symbols
void set_english_lower_character_alphabet(char * p_alphabet)
{
    p_alphabet[0] = 'a';
    p_alphabet[1] = 'b';
    p_alphabet[2] = 'c';
    p_alphabet[3] = 'd';
    p_alphabet[4] = 'e';
    p_alphabet[5] = 'f';
    p_alphabet[6] = 'g';
    p_alphabet[7] = 'h';
    p_alphabet[8] = 'i';
    p_alphabet[9] = 'j';
    p_alphabet[10] = 'k';
    p_alphabet[11] = 'l';
    p_alphabet[12] = 'm';
    p_alphabet[13] = 'n';
    p_alphabet[14] = 'o';
    p_alphabet[15] = 'p';
    p_alphabet[16] = 'q';
    p_alphabet[17] = 'r';
    p_alphabet[18] = 's';
    p_alphabet[19] = 't';
    p_alphabet[20] = 'u';
    p_alphabet[21] = 'v';
    p_alphabet[22] = 'w';
    p_alphabet[23] = 'x';
    p_alphabet[24] = 'y';
    p_alphabet[25] = 'z';
}
//===========================================================================================================
/// @brief set_english_symbols_typical_frequence stored typical frequencies of symbols of english text
/// @param p_array array af symbols english alphabet
void set_english_symbols_typical_frequence(double * p_array)
{
    p_array[0] = 0.08167;
    p_array[1] = 0.01492;
    p_array[2] = 0.02782;
    p_array[3] = 0.04253;
    p_array[4] = 0.12702;
    p_array[5] = 0.02228;
    p_array[6] = 0.02015;
    p_array[7] = 0.06094;
    p_array[8] = 0.06966;
    p_array[9] = 0.00153;
    p_array[10] = 0.00772;
    p_array[11] = 0.04025;
    p_array[12] = 0.02406;
    p_array[13] = 0.06749;
    p_array[14] = 0.07507;
    p_array[15] = 0.01929;
    p_array[16] = 0.00095;
    p_array[17] = 0.05987;
    p_array[18] = 0.06327;
    p_array[19] = 0.09056;
    p_array[20] = 0.02758;
    p_array[21] = 0.00978;
    p_array[22] = 0.0236;
    p_array[23] = 0.0015;
    p_array[24] = 0.01974;
    p_array[25] = 0.00074;
}
//===========================================================================================================
/// @brief set array of indexes of char english symbols, for fast access to indexes of engish symbols
/// @param [out] p_array array for store
/// @param p_alphabet char array symbols of alphabet
/// @param alphabet_power quantity symbols of alphabet
void set_array_numbers_of_charasters(unsigned int *p_array, char *p_alphabet, const int alphabet_power)
{
    int characsters_count;
    for(characsters_count = 0; characsters_count < alphabet_power; ++characsters_count) {
        p_array[(unsigned int)p_alphabet[characsters_count]] = characsters_count;
    }
}
//===========================================================================================================
/// @brief get_vigenere_encrypted_char return vigenere encrypted with 'p_key' char related
/// to source char 'ch_source'
/// @param ch_source source char symbol
/// @param position position of the source char in text
/// @param p_key key of vigenere cipher
/// @param size_of_key size of the key
char get_vigenere_encrypted_char(const char ch_source, const int position,
                                 const char * p_key, const int size_of_key)
{
    char ch_to_return = 0x00;
    int i_column = p_array_numbers_of_character[(unsigned int)tolower((int)ch_source)];
    int i_row = p_array_numbers_of_character[(unsigned int)tolower((int)p_key[position%size_of_key])];
    if (i_column < ALPHABET_POWER && i_row < ALPHABET_POWER) {
        ch_to_return = p_vigenere_square[i_column][i_row];
    }
    else {
    }
    return ch_to_return;
}
//===========================================================================================================
/// @brief get_vigenere_encrypted_char return vigenere encrypted with 'p_key' char related
/// to source char 'ch_source'
/// @param ch_source source char symbol
/// @param position position of the source char in text
/// @param p_key key of vigenere cipher
/// @param size_of_key size of the key
char get_vigenere_decrypted_char(const char ch_source, const int position,
                                 const char * p_key, const int size_of_key)
{
    char ch_to_return = ch_source;
    int i_row = p_array_numbers_of_character[(unsigned int)tolower((int)p_key[position%size_of_key])];
    int i_column = 0;
    for(i_column = 0; i_column <ALPHABET_POWER; ++i_column) {
        if(p_vigenere_square[i_column][i_row] == ch_source) {
            ch_to_return = p_alphabet[i_column];
            break;
        }
    }
    return ch_to_return;
}
//===========================================================================================================
/// @brief convolution_with_basic_sequence evaluate correlation (convolution) sequence
/// from researched sequence and basic sequence, result sequence will be stored in 'p_researched_sequence'
/// @param [out] p_researched_sequence researched sequence with unknown values,
/// @param p_basic_sequence sequence with previously stored values
/// @param p_temp buffer array for evaluate
/// @param size size of sequences
void convolution_with_basic_sequence(double * p_researched_sequence,
                                     double * p_basic_sequence,
                                     double * p_temp,
                                     const int size)
{
    int shift, element_count;
    for(shift = 0; shift < size; ++shift) {
        p_temp[shift] = 0;
        for(element_count = 0; element_count < size; ++element_count) {
            p_temp[shift] += p_researched_sequence[(shift + element_count)%size] *
                             p_basic_sequence[element_count];
        }
        p_temp[shift] /= (double)size;
    }
    for(element_count = 0; element_count < size; ++element_count) {
        p_researched_sequence[element_count] = p_temp[element_count];
    }
}
//===========================================================================================================
/// @brief correlation_research do research result of correlation, if two sequences were correlated
/// the result passed as 'pd_sequence' have clearly maximum 'p_index_maximum' and good
/// ratio maximum : middle_level
/// also evaluated middle difference of elements and middle level 'p_sigma_middle_level'
/// @param 'pd_sequence' previously evaluated correlation sequence
/// @param lenght_sequence lenght array of 'pd_sequence'
/// @param [out] p_max_to_middle_others_relation ratio maximum : middle level
/// @param [out] p_sigma_middle_level middle difference elements from middle level
/// @param [out] p_index_maximum index of maximum
void correlation_research(double * pd_sequence,
                          int lenght_sequence,
                          double * p_max_to_middle_others_relation,
                          double * p_sigma_middle_level,
                          int * p_index_maximum)
{
    double maximum = -1.0, middle_level = 0.0;
    int index_maximum = -1, symbol_count;

    for(symbol_count = 0; symbol_count < lenght_sequence; ++symbol_count) {
        if (maximum < pd_sequence[symbol_count]) {
            index_maximum = symbol_count;
            maximum = pd_sequence[symbol_count];
        }
    }
    *p_index_maximum = index_maximum;
    for(symbol_count = 0; symbol_count < lenght_sequence; ++symbol_count) {
        if (index_maximum != symbol_count) {
            middle_level += pd_sequence[symbol_count];
        }
    }
    middle_level /= (double)(lenght_sequence-1);

    *p_sigma_middle_level = 0.0;
    for(symbol_count = 0; symbol_count < lenght_sequence; ++symbol_count) {
        if (index_maximum != symbol_count) {
            *p_sigma_middle_level += fabs(pd_sequence[symbol_count] - middle_level);
        }
    }

    *p_sigma_middle_level /= (double)(lenght_sequence-1);
    *p_max_to_middle_others_relation = maximum / middle_level;
    if(maximum / middle_level > 3.0) {
        printf("correlation_research: warning, ratio maximum:middle_level = %f is too big\n", maximum / middle_level);
    }
}
//===========================================================================================================
/// @brief get_key_from_file read key from file, stored key lenght,
/// key length is limited by 'MAX_KEY_SIZE', if we have
/// in the file a longer key, we use first MAX_KEY_SIZE symbols to it, if the 'p_source_file' is empty
/// of permission denied or not exist we store a default key
/// @param p_source_file filename of file with key
/// @return 0 if success, negative 0 if success, negative value if an error occurred
int get_key_from_file(const char * p_source_file)
{
    int i_to_return = 0, symbols_count = 0;
    char ch_from_file;
    FILE * p_file_source = NULL;
    //p_key = "abcdefgijklm";
    strncpy(p_key, "abcdefgijklm", MAX_KEY_SIZE);
    p_file_source = fopen(p_source_file, "r");
    if (p_file_source == NULL) {
        perror("get_key_from_file fopen failed");
        i_to_return = -1;
        return i_to_return;
    }
    else {
        while((ch_from_file = fgetc(p_file_source)) != EOF) {
            if(isalpha(ch_from_file)) {
                ch_from_file = tolower((int)ch_from_file);
                if (symbols_count >= MAX_KEY_SIZE) {
                    break;
                }
                p_key[symbols_count] = ch_from_file;
                symbols_count++;
            }
        }
        if (fclose(p_file_source)) {
            perror("get_key_from_file fclose failed");
            i_to_return = -1;
            return i_to_return;
        }
        if (!symbols_count) {
            fprintf(stderr, "no symbols in source file\n");
            i_to_return = -1;
            return i_to_return;
        }
        else {
            p_key[symbols_count] = '\0';
        }
    }
    key_size = strlen(p_key);
    return i_to_return;
}
//===========================================================================================================
/// @brief print_key print vigenere key stored in variable 'p_key' and it size
void print_key()
{
    printf("%s, size = %d\n", p_key, key_size);
}
//===========================================================================================================
/// @brief test_minimal_text_file_lenght will be test frequence analiz algorithm for decrypting
/// vigenere cipher crypted text, as a result function stored minimal lenght of successfully decrypted
/// text into variable pointed by p_result_lenght
/// @param pch_text_file filename of text with plain text
/// @param [out] p_result_lenght pointer to variable that will be get as test result
/// @return 0 if success, negative 0 if success, negative value if an error occurred
int test_minimal_text_file_lenght(const char * pch_text_file, int * p_result_lenght)
{
    const int QUANTITY_SHOW_WAIT_ITTERATIONS = 1000;
    int i_to_return = 0, lenght_encrypted_text_array = 0, position = 0, truncated_lenght;
    char ch_from_file;
    FILE * p_text_file = NULL;
    char * pch_encrypted_text_array = NULL;
    const char * p_name_encrypted_file = "./encrypted/test_minimal_encrypted";
    *p_result_lenght = -1;
    if (vigenere_encrypt(pch_text_file, p_name_encrypted_file) < 0) {
        fprintf(stderr, "vigenere_decrypt failed\n");
        i_to_return = -1;
        return i_to_return;
    }
    p_text_file = fopen(p_name_encrypted_file, "r");
    if (p_text_file == NULL) {
        perror("test_minimal_text_file_lenght failed");
        i_to_return = -1;
    }
    else {
        while((ch_from_file = fgetc(p_text_file)) != EOF) {
            lenght_encrypted_text_array++;
        }
        pch_encrypted_text_array = (char*)malloc(lenght_encrypted_text_array);
        if (pch_encrypted_text_array == NULL) {
            perror("malloc failed");
            if (fclose(p_text_file)) {
                perror("fclose failed");
            }
            i_to_return = -1;
            return i_to_return;
        }
        if(fseek(p_text_file, 0, SEEK_SET)) {
            perror("fseek failed");
            i_to_return = -1;
            free(pch_encrypted_text_array);
            return i_to_return;
        }
        else {
            while((ch_from_file = fgetc(p_text_file)) != EOF) {
                pch_encrypted_text_array[position] = ch_from_file;
                position++;
            }
            if (fclose(p_text_file)) {
                perror("fclose failed");
                free(pch_encrypted_text_array);
                i_to_return = -1;
                return i_to_return;
            }
            for (truncated_lenght = lenght_encrypted_text_array; truncated_lenght > ALPHABET_POWER; --truncated_lenght) {
                if (truncated_lenght > QUANTITY_SHOW_WAIT_ITTERATIONS) {
                    if (truncated_lenght % QUANTITY_SHOW_WAIT_ITTERATIONS == 0) {
                        printf("remain processing %d itterations\n", truncated_lenght);
                        fflush(stdout);
                    }
                }
                if (get_guess_key_from_text_array(pch_encrypted_text_array, truncated_lenght) < 0) {
                    fprintf(stderr, "get_guess_key_from_text_array failed\n");
                    free(pch_encrypted_text_array);
                    i_to_return = -1;
                    return i_to_return;
                }
                else {
                    if (strcmp(p_guess_key, p_key)) {
                        *p_result_lenght = truncated_lenght;
                        break;
                    }
                }
            }
            if (*p_result_lenght == -1) {
                fprintf(stderr, "could not store minimal lenght of text\n");
                i_to_return = -1;
                return i_to_return;
            }
        }
        free(pch_encrypted_text_array);
    }
    return i_to_return;
}
//===========================================================================================================
/// @brief get_guess_key_from_text_array get guess key from char array 'pch_encrypted_text_array' with lenght
/// 'lenght_encrypted_text_array' result guess key will be stored in p_guess_key
/// @param pch_encrypted_text_array encrypted text to get guess key from it
/// @param lenght_encrypted_text_array lenght of encrypted text array 'pch_encrypted_text_array'
/// @return 0 if success, negative value if an error occurred
int get_guess_key_from_text_array(char * pch_encrypted_text_array, const int lenght_encrypted_text_array)
{
    int i_to_return = 0;
    int index_mod_with_maximum, index_key_symbol;
    int lenght_guess_key;
    reset_array_symbols_frequence_into_crypted_text();
    if (set_subarrays_symbols_from_char_array(pch_encrypted_text_array, lenght_encrypted_text_array) < 0) {
        fprintf(stderr, "set_subarrays_symbols_frequence_into_crypted_text failed\n");
        i_to_return = -1;
        return i_to_return;
    }

    if(convolution_research(&index_mod_with_maximum, &index_key_symbol) < 0) {
        fprintf(stderr,"convolution_research failed");
    }

    lenght_guess_key = index_mod_with_maximum + 1;
    if (lenght_guess_key > MAX_KEY_SIZE) {
        fprintf(stderr, "lenght_guess_key: %d, greater than MAX_KEY_SIZE: %d\n", lenght_guess_key, MAX_KEY_SIZE);
        i_to_return = -1;
        return i_to_return;
    }
    p_guess_key[0] = p_alphabet[index_key_symbol];
    p_guess_key[lenght_guess_key] = '\0';
    if (lenght_guess_key != 1) {
        double ** pd_symbols_frequence_guess_key = set_key_search_data(lenght_guess_key);
        if (pd_symbols_frequence_guess_key == NULL) {
            fprintf(stderr, "set_key_search_data failed");
            i_to_return = -1;
            return i_to_return;
        }
        if (set_subarrays_symbols_guess_key_from_char_array(pch_encrypted_text_array,
                                                            lenght_encrypted_text_array,
                                                            pd_symbols_frequence_guess_key,
                                                            lenght_guess_key) < 0) {

            fprintf(stderr, "set_subarrays_symbols_guess_key_from_char_array failed");
            if (pd_symbols_frequence_guess_key != NULL) {
                free_key_search_data(pd_symbols_frequence_guess_key, lenght_guess_key);
            }
            i_to_return = -1;
            return i_to_return;
        }

        if (convolution_research_rested_key_symbols(pd_symbols_frequence_guess_key, lenght_guess_key) < 0) {
            fprintf(stderr, "convolution_research_rested_key_symbols failed");
            if (pd_symbols_frequence_guess_key != NULL) {
                free_key_search_data(pd_symbols_frequence_guess_key, lenght_guess_key);
            }
            i_to_return = -1;
            return i_to_return;
        }
        if (pd_symbols_frequence_guess_key != NULL) {
            free_key_search_data(pd_symbols_frequence_guess_key, lenght_guess_key);
        }
    }
    return i_to_return;
}
//===========================================================================================================























