#include <stdlib.h>
#include <math.h>
//===========================================================================================================
#include "vigenere_encrypt.h"
//===========================================================================================================
const int ALPHABET_POWER = 26;
const int SIZE_ARRAY_NUMBERS_OF_CHARACTERS = 256;
char * p_alphabet;// = (char*)malloc(ALPHABET_POWER*sizeof(char));
char ** p_vigenere_square;// = (char**)malloc(ALPHABET_POWER*sizeof(int));
unsigned int * p_array_numbers_of_character;//[SIZE_ARRAY_NUMBERS_OF_CHARACTERS];
const char * p_key = "asdfqwertyr";
//const char * p_key = "lemon";
char * p_guess_key;
const int SIZE_OF_KEY = 11;
const int MAX_KEY_SIZE = 12;
//===========================================================================================================
//double max_to_middle_others_relation;
//double sigma_middle_level;
int lenght_guess_key;
//===========================================================================================================
double * pd_symbols_frequence_part_of_one;
double ** pd_symbols_frequence_into_crypted_text;

double * pd_convolution_result;
double * pd_convolution_buffer;
//===========================================================================================================
void set_data()
{
    p_alphabet = (char*)malloc(ALPHABET_POWER*sizeof(char));
    pd_symbols_frequence_part_of_one = (double*)malloc(ALPHABET_POWER*sizeof(double));
    pd_convolution_result = (double*)malloc(ALPHABET_POWER*sizeof(double));
    pd_convolution_buffer = (double*)malloc(ALPHABET_POWER*sizeof(double));
    p_array_numbers_of_character = (unsigned int*)malloc(SIZE_ARRAY_NUMBERS_OF_CHARACTERS*sizeof(int));
    p_vigenere_square = (char**)malloc(ALPHABET_POWER*sizeof(int));
    pd_symbols_frequence_into_crypted_text = (double**)malloc(MAX_KEY_SIZE*sizeof(int));
    p_guess_key = (char*)malloc((MAX_KEY_SIZE+1)*sizeof(char));

    for(int row_count = 0; row_count < ALPHABET_POWER; ++row_count) {
        p_vigenere_square[row_count] = (char*)malloc(ALPHABET_POWER);
//        pd_symbols_frequence_into_crypted_text[row_count] = (double*)malloc(MAX_KEY_SIZE*sizeof(double));
//        if(p_vigenere_square[row_count] == 0) {
//            printf("p_vigenere_square[%d]=0!!!", row_count);
//        }
    }
    for (int mod_count = 0; mod_count < MAX_KEY_SIZE; ++mod_count) {
        pd_symbols_frequence_into_crypted_text[mod_count] = (double*)malloc(ALPHABET_POWER*sizeof(double));
    }

    set_english_lower_character_alphabet(p_alphabet);
    set_english_symbols_typical_frequence(pd_symbols_frequence_part_of_one);
    set_vigenere_square(p_vigenere_square, p_alphabet, ALPHABET_POWER);
    set_array_numbers_of_charasters(p_array_numbers_of_character, p_alphabet, ALPHABET_POWER);
//    autocorrelation_research(pd_convolution_result, pd_symbols_frequence_part_of_one, pd_convolution_buffer,
//                             ALPHABET_POWER, &max_to_middle_others_relation, &sigma_middle_level);
}
//===========================================================================================================
void free_data()
{
    free(p_alphabet);
    for(int row_count = 0; row_count < ALPHABET_POWER; ++row_count) {
        free(p_vigenere_square[row_count]);
        //free(pd_symbols_frequence_into_crypted_text[row_count]);
    }
    for (int mod_count = 0; mod_count < MAX_KEY_SIZE; ++mod_count) {
        free(pd_symbols_frequence_into_crypted_text[mod_count]);
        //pd_symbols_frequence_into_crypted_text[mod_count] = (double*)malloc(ALPHABET_POWER*sizeof(double));
    }
    free(p_vigenere_square);
    free(p_array_numbers_of_character);
    free(pd_symbols_frequence_part_of_one);
    free(pd_convolution_result);
    free(pd_convolution_buffer);
    free(pd_symbols_frequence_into_crypted_text);
    free(p_guess_key);
}
//===========================================================================================================
void vigenere_encrypt(const char * p_source_file, const char * p_dest_file) {
    //printf("source %s, dest %s \n", p_source_file, p_dest_file);
//    const int ALPHABET_POWER = 26;
//    const int SIZE_ARRAY_NUMBERS_OF_CHARACTERS = 256;
//    p_alphabet = (char*)malloc(ALPHABET_POWER*sizeof(char));
//    p_vigenere_square = (char**)malloc(ALPHABET_POWER*sizeof(int));
//    p_array_numbers_of_character = (unsigned int*)malloc(SIZE_ARRAY_NUMBERS_OF_CHARACTERS*sizeof(int));
//    unsigned int p_array_numbers_of_character[SIZE_ARRAY_NUMBERS_OF_CHARACTERS];

//    for(int row_count = 0; row_count < ALPHABET_POWER; ++row_count) {
//        p_vigenere_square[row_count] = (char*)malloc(ALPHABET_POWER);
//        if(p_vigenere_square[row_count] == 0) {
//            printf("p_vigenere_square[%d]=0!!!", row_count);
//        }
//    }
//    set_english_lower_character_alphabet(p_alphabet);
//    set_vigenere_square(p_vigenere_square, p_alphabet, ALPHABET_POWER);
//    set_array_numbers_of_charasters(p_array_numbers_of_character, p_alphabet, ALPHABET_POWER);
//    for(int row_count = 0; row_count < ALPHABET_POWER; ++row_count) {
//        printf("%d:, %s\n",row_count, p_vigenere_square[row_count]);
//    }
    
    FILE * p_file_source = fopen(p_source_file, "r");
    FILE * p_file_dest = fopen(p_dest_file, "w");
    if (p_file_source == NULL || p_dest_file == NULL) {
        printf("cannot open file");
        return;
    }
    else {
        char ch_from_file;
        //char to_file;
        int position=0;
        while((ch_from_file = fgetc(p_file_source)) != EOF) {
            if(isalpha(ch_from_file)) {
                ch_from_file = get_vigenere_encrypted_char(ch_from_file, position, p_key, SIZE_OF_KEY);
                position++;
            }
            putc(ch_from_file, p_file_dest);
        }
    }
    fclose(p_file_source);
    fclose(p_file_dest);
    
    
//    free(p_alphabet);
//    for(int row_count = 0; row_count < ALPHABET_POWER; ++row_count) {
//        free(p_vigenere_square[row_count]);
//    }
//    free(p_vigenere_square);
//    free(p_array_numbers_of_character);
}
//===========================================================================================================
void vigenere_decrypt(const char * p_source_file, const char * p_dest_file)
{
    FILE * p_file_source = fopen(p_source_file, "r");
    FILE * p_file_dest = fopen(p_dest_file, "w");
    if (p_file_source == NULL || p_dest_file == NULL) {
        printf("cannot open file");
        return;
    }
    else {
        char ch_from_file;
        //char to_file;
        int position=0;
        while((ch_from_file = fgetc(p_file_source)) != EOF) {
            if(isalpha(ch_from_file)) {
                ch_from_file = get_vigenere_decrypted_char(ch_from_file, position, p_key, SIZE_OF_KEY);
                position++;
            }
            putc(ch_from_file, p_file_dest);
        }
    }
    fclose(p_file_source);
    fclose(p_file_dest);
}
//===========================================================================================================
void reset_array_symbols_frequence_into_crypted_text()
{
    for (int mod_count = 0; mod_count < MAX_KEY_SIZE; ++mod_count) {
        for (int symbol_count = 0; symbol_count < ALPHABET_POWER; ++symbol_count) {
            pd_symbols_frequence_into_crypted_text[mod_count][symbol_count] = 0.0;
        }
    }
}
//===========================================================================================================
void frequence_analiz(const char * p_source_file, const char * p_dest_file)
{

    reset_array_symbols_frequence_into_crypted_text();
    FILE * p_file_source = fopen(p_source_file, "r");
    //FILE * p_file_dest = fopen(p_dest_file, "w");
    if (p_file_source == NULL) {
        printf("cannot open file");
        return;
    }
    else {
        char ch_from_file;
        //char to_file;
        //int position=0;
        int symbols_count = 0;
        int i_alphabet_char_index;
//        int modes_summary[MAX_KEY_SIZE];
//        for (int mod = 0; mod < MAX_KEY_SIZE; ++mod) {
//            modes_summary[mod] = 0;
//        }
        while((ch_from_file = fgetc(p_file_source)) != EOF) {
            if(isalpha(ch_from_file)) {
                ch_from_file = tolower((int)ch_from_file);
                i_alphabet_char_index = p_array_numbers_of_character[(unsigned int)ch_from_file];
                //if (symbols_count % 5 == 0) {
                //    pd_symbols_frequence_into_crypted_text[i_alphabet_char_index][4] += 1.0;
                //}
                for(int mod = 1; mod <= MAX_KEY_SIZE; ++mod) {
                    if (symbols_count % mod == 0) {
                        //modes_summary[mod-1]++;
                        //printf("symbols_count=%d, mod=%d \n", symbols_count, mod-1);
                        //printf("f[%c][%d]++\n", ch_from_file, mod - 1);
                        pd_symbols_frequence_into_crypted_text[mod-1][i_alphabet_char_index] += 1.0;
                    }
                }
                symbols_count++;
            }
            //position++;
        }
//        for (int mod = 0; mod < MAX_KEY_SIZE; ++mod) {
//            printf("modes_summary[%d]=%d will be %d\n", mod, modes_summary[mod], (int)((double)symbols_count/(double)(mod+1)));
//        }
        double divider = 1.0;

        //printf("symbols_count=%d\n",symbols_count);
//        for (int symbol_count = 0; symbol_count < ALPHABET_POWER; ++symbol_count) {
//            for (int mod_count = 0; mod_count < MAX_KEY_SIZE; ++mod_count) {
//                divider = (double)symbols_count/(double)(mod_count+1);
//                pd_symbols_frequence_into_crypted_text[symbol_count][mod_count] /= divider;
//                //printf("fr[%c][%d]=%f ", p_alphabet[symbol_count], mod_count,
//                //       pd_symbols_frequence_into_crypted_text[symbol_count][mod_count]);

//            }
//            //printf("\n");
//        }
        for (int mod_count = 0; mod_count < MAX_KEY_SIZE; ++mod_count) {
            divider = (double)symbols_count/(double)(mod_count+1);
            for (int symbol_count = 0; symbol_count < ALPHABET_POWER; ++symbol_count) {
                pd_symbols_frequence_into_crypted_text[mod_count][symbol_count] /= divider;
//                printf("f[%d][%c]=%f ", mod_count, p_alphabet[symbol_count],
//                       pd_symbols_frequence_into_crypted_text[mod_count][symbol_count]);
            }
//            printf("\n");
        }

//        printf("\n\n");

        for (int mod_count = 0; mod_count < MAX_KEY_SIZE; ++mod_count) {
            convolution_with_basic_sequence(pd_symbols_frequence_into_crypted_text[mod_count],
                                            pd_symbols_frequence_part_of_one,
                                            pd_convolution_buffer,
                                            ALPHABET_POWER);
        }
        double array_max_to_middle_others_relation[MAX_KEY_SIZE];
        double array_sigma_middle_level[MAX_KEY_SIZE];
        int array_index_maximum[MAX_KEY_SIZE];

//        printf("autocorr max_to_middle = %f, sigma_middle_level = %f\n", max_to_middle_others_relation,
//                sigma_middle_level);
        double maximum_in_modes = -1.0;
        int index_mod_with_maximum;
        int index_key_symbol;
        for (int mod_count = 0; mod_count < MAX_KEY_SIZE; ++mod_count) {
            //printf("mod=%d ", mod_count);
            correlation_research(pd_symbols_frequence_into_crypted_text[mod_count],
                                 ALPHABET_POWER,
                                 &array_max_to_middle_others_relation[mod_count],
                                 &array_sigma_middle_level[mod_count],
                                 &array_index_maximum[mod_count]);
//            printf("max_to_middle[%d]=%f, sigma[%d]=%f, i_max= %d\n",
//                   mod_count, array_max_to_middle_others_relation[mod_count],
//                   mod_count, array_sigma_middle_level[mod_count],
//                   array_index_maximum[mod_count]);
            if (maximum_in_modes <  array_max_to_middle_others_relation[mod_count]) {
                maximum_in_modes = array_max_to_middle_others_relation[mod_count];
                index_mod_with_maximum = mod_count;
                index_key_symbol = array_index_maximum[mod_count];
//printf("maximum_in_modes=%f, mod_count=%d", maximum_in_modes, mod_count);
            }
//            if (maximum_in_modes < pd_symbols_frequence_into_crypted_text[mod_count]
//                                                                         [array_index_maximum[mod_count]]) {
//                maximum_in_modes = pd_symbols_frequence_into_crypted_text[mod_count]
//                                                                         [array_index_maximum[mod_count]];
//                index_mod_with_maximum = mod_count;
//                index_key_symbol = array_index_maximum[mod_count];
//            }
        }
//        printf("guess lenght key = %d, first symbol of key is %c \n", index_mod_with_maximum + 1,
//                                                                  p_alphabet[index_key_symbol]);
        lenght_guess_key = index_mod_with_maximum + 1;
        p_guess_key[0] = p_alphabet[index_key_symbol];
        if (lenght_guess_key != 1) {
            double ** pd_symbols_frequence_guess_key;
            pd_symbols_frequence_guess_key = (double**)malloc((lenght_guess_key-1)*sizeof(int));
            for (int key_symbol_count = 0; key_symbol_count < (lenght_guess_key-1); ++key_symbol_count) {
                pd_symbols_frequence_guess_key[key_symbol_count] = (double*)malloc(ALPHABET_POWER*sizeof(double));
            }
            fseek(p_file_source, 0, SEEK_SET);
            int rest_from_guess_key;
            symbols_count = 0;
            while((ch_from_file = fgetc(p_file_source)) != EOF) {
                if(isalpha(ch_from_file)) {
                    ch_from_file = tolower((int)ch_from_file);
                    i_alphabet_char_index = p_array_numbers_of_character[(unsigned int)ch_from_file];
                    //if (symbols_count % 5 == 0) {
                    //    pd_symbols_frequence_into_crypted_text[i_alphabet_char_index][4] += 1.0;
                    //}
                    //for(int key_symbol_count = 0; key_symbol_count < (lenght_guess_key-1); ++key_symbol_count) {
                    rest_from_guess_key = (symbols_count -1) % lenght_guess_key;

                    if (rest_from_guess_key >= 0 && (rest_from_guess_key != (lenght_guess_key-1))) {
                        //printf("rest = %d", rest_from_guess_key);
                        pd_symbols_frequence_guess_key[rest_from_guess_key][i_alphabet_char_index] += 1.0;
                    }
//                    if (symbols_count % mod == 0) {
//                        //modes_summary[mod-1]++;
//                        //printf("symbols_count=%d, mod=%d \n", symbols_count, mod-1);
//                        //printf("f[%c][%d]++\n", ch_from_file, mod - 1);
//                        pd_symbols_frequence_into_crypted_text[mod-1][i_alphabet_char_index] += 1.0;
//                    }
                    //}
                    symbols_count++;
                }
                //position++;
            }
            divider = (double)symbols_count/(double)(lenght_guess_key);
            for (int key_symbol_count = 0; key_symbol_count < (lenght_guess_key-1); ++key_symbol_count) {
                for (int symbol_count = 0; symbol_count < ALPHABET_POWER; ++symbol_count) {
                    pd_symbols_frequence_guess_key[key_symbol_count][symbol_count] /= divider;
    //                printf("f[%d][%c]=%f ", mod_count, p_alphabet[symbol_count],
    //                       pd_symbols_frequence_into_crypted_text[mod_count][symbol_count]);
                }
    //            printf("\n");
            }
            double max_to_middle_others_relation;
            double sigma_middle_level;
            int index_maximum;
            for (int key_symbol_count = 0; key_symbol_count < lenght_guess_key - 1; ++key_symbol_count) {
                convolution_with_basic_sequence(pd_symbols_frequence_guess_key[key_symbol_count],
                                                pd_symbols_frequence_part_of_one,
                                                pd_convolution_buffer,
                                                ALPHABET_POWER);
                correlation_research(pd_symbols_frequence_guess_key[key_symbol_count],
                                     ALPHABET_POWER,
                                     &max_to_middle_others_relation,
                                     &sigma_middle_level,
                                     &index_maximum);
                //printf("index maximum = %d\n", index_maximum);
                p_guess_key[key_symbol_count+1] = p_alphabet[index_maximum];
                //printf("symbol number %d of key is:%c\n", key_symbol_count+2, p_alphabet[index_maximum]);

            }
            p_guess_key[lenght_guess_key] = '\0';
            printf("the key is \'%s\' \n", p_guess_key);
            for (int key_symbol_count = 0; key_symbol_count < (lenght_guess_key-1); ++key_symbol_count) {
                free(pd_symbols_frequence_guess_key[key_symbol_count]);
            }
            free(pd_symbols_frequence_guess_key);
        }
//        for (int mod_count = 0; mod_count < MAX_KEY_SIZE; ++mod_count) {
//            for (int symbol_count = 0; symbol_count < ALPHABET_POWER; ++symbol_count) {
//                //pd_symbols_frequence_into_crypted_text[mod_count][symbol_count] /= divider;
//                printf("f[%d][%c]=%f ", mod_count, p_alphabet[symbol_count],
//                       pd_symbols_frequence_into_crypted_text[mod_count][symbol_count]);
//            }
//            printf("\n");
//        }
//        double sum_at_mod = 0.0;
//        for (int mod_count = 0; mod_count < MAX_KEY_SIZE; ++mod_count) {
//            sum_at_mod = 0;
//            for (int symbol_count = 0; symbol_count < ALPHABET_POWER; ++symbol_count) {
//                sum_at_mod += pd_symbols_frequence_into_crypted_text[symbol_count][mod_count];
//            }
//            printf("sum_at_mod=%f, mod=%d", sum_at_mod, mod_count);
//        }
    }
    fclose(p_file_source);
}
//===========================================================================================================
void set_vigenere_square(char ** p_vigenere_square, char * p_alphabet, const int alphabet_power)
{
    for(int row_count = 0; row_count < alphabet_power; ++row_count) {
        for(int column_count = 0; column_count < alphabet_power; ++column_count) {
            p_vigenere_square[row_count][column_count] = p_alphabet[(column_count + row_count) % alphabet_power];
        }
    }
}
//===========================================================================================================
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
void set_array_numbers_of_charasters(unsigned int *p_array, char *p_alphabet, const int alphabet_power)
{
    for(int characsters_count = 0; characsters_count < alphabet_power; ++characsters_count) {
        p_array[(unsigned int)p_alphabet[characsters_count]] = characsters_count;
        //printf(" p_array[(unsigned int) %d]=%d\n", (unsigned int)p_alphabet[characsters_count], characsters_count);
    }
}
//===========================================================================================================
char get_vigenere_encrypted_char(const char ch_source, const int position,
                                 const char * p_key, const int size_of_key)
{
    char ch_to_return = 0x00;
    int i_column = p_array_numbers_of_character[(unsigned int)tolower((int)ch_source)];
    int i_row = p_array_numbers_of_character[(unsigned int)tolower((int)p_key[position%size_of_key])];
    if (i_column < ALPHABET_POWER && i_row < ALPHABET_POWER) {
        ch_to_return = p_vigenere_square[i_column][i_row];
//        printf("source char = %c \n", (char)(unsigned int)tolower((int)ch_source));
//        printf("ch_to_return=%c, i_column=%d i_row=%d \n", ch_to_return, i_column, i_row);
    }
    else {
//        printf("i_column > ALPHABET_POWER or i_row > ALPHABET_POWER");
    }
    //printf("%c=%c\n", ch_source, ch_to_return);
    //printf("p_key[%d mod %d = %d]=%c\n", position, size_of_key,position%size_of_key, p_key[position%size_of_key]);
    return ch_to_return;
}
//===========================================================================================================
char get_vigenere_decrypted_char(const char ch_source, const int position,
                                 const char * p_key, const int size_of_key)
{
    char ch_to_return = 0x00;
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
void convolution_with_basic_sequence(double * p_researched_sequence,
                                     double * p_basic_sequence,
                                     double * p_temp,
                                     const int size)
{
    for(int shift = 0; shift < size; ++shift) {
        p_temp[shift] = 0;
        for(int element_count = 0; element_count < size; ++element_count) {
            p_temp[shift] += p_researched_sequence[(shift + element_count)%size] *
                             p_basic_sequence[element_count];
        }
        p_temp[shift] /= (double)size;
    }
    for(int element_count = 0; element_count < size; ++element_count) {
        p_researched_sequence[element_count] = p_temp[element_count];
    }
}
//===========================================================================================================
void test_convolution_with_basic_sequence()
{
    double researched_sequence[ALPHABET_POWER], basic_sequence[ALPHABET_POWER], temp[ALPHABET_POWER];
    researched_sequence[0]=0.011933;
    researched_sequence[1]=0.000663;
    researched_sequence[2]=0.060329;
    researched_sequence[3]=0.068947;
    researched_sequence[4]=0.089499;
    researched_sequence[5]=0.025855;
    researched_sequence[6]=0.011270;
    researched_sequence[7]=0.027181;
    researched_sequence[8]=0.000000;
    researched_sequence[9]=0.015911;
    researched_sequence[10]=0.001326;
    researched_sequence[11]=0.080217;
    researched_sequence[12]=0.013259;
    researched_sequence[13]=0.019226;
    researched_sequence[14]=0.052373;
    researched_sequence[15]=0.133254;
    researched_sequence[16]=0.020552;
    researched_sequence[17]=0.020552;
    researched_sequence[18]=0.077566;
    researched_sequence[19]=0.054362;
    researched_sequence[20]=0.000000;
    researched_sequence[21]=0.012596;
    researched_sequence[22]=0.037788;
    researched_sequence[23]=0.019889;
    researched_sequence[24]=0.067621;
    researched_sequence[25]=0.078229;

    basic_sequence[0] = 0.08167;
    basic_sequence[1] = 0.01492;
    basic_sequence[2] = 0.02782;
    basic_sequence[3] = 0.04253;
    basic_sequence[4] = 0.12702;
    basic_sequence[5] = 0.02228;
    basic_sequence[6] = 0.02015;
    basic_sequence[7] = 0.06094;
    basic_sequence[8] = 0.06966;
    basic_sequence[9] = 0.00153;
    basic_sequence[10] = 0.00772;
    basic_sequence[11] = 0.04025;
    basic_sequence[12] = 0.02406;
    basic_sequence[13] = 0.06749;
    basic_sequence[14] = 0.07507;
    basic_sequence[15] = 0.01929;
    basic_sequence[16] = 0.00095;
    basic_sequence[17] = 0.05987;
    basic_sequence[18] = 0.06327;
    basic_sequence[19] = 0.09056;
    basic_sequence[20] = 0.02758;
    basic_sequence[21] = 0.00978;
    basic_sequence[22] = 0.0236;
    basic_sequence[23] = 0.0015;
    basic_sequence[24] = 0.01974;
    basic_sequence[25] = 0.00074;

//    for(int i = 0; i<ALPHABET_POWER; ++i) {
//        researched_sequence[i] = 0;
//        basic_sequence[i] = 0;
//    }
//    researched_sequence[11] = 1;
//    basic_sequence[0] = 1;
    convolution_with_basic_sequence(researched_sequence, basic_sequence, temp, ALPHABET_POWER);
    for(int i = 0; i < ALPHABET_POWER; ++i) {
        printf("researched_sequence[%d]=%f\n", i, researched_sequence[i]);
    }
}
//===========================================================================================================
void autocorrelation_research(double * pd_convolution_result,
                              double * pd_basic_sequence,
                              double * pd_convolution_buffer,
                              int lenght_sequence,
                              double * p_max_to_middle_others_relation,
                              double * p_sigma_middle_level)
{
    for(int symbol_count = 0; symbol_count < lenght_sequence; ++symbol_count) {
        pd_convolution_result[symbol_count] = pd_basic_sequence[symbol_count];
    }
    convolution_with_basic_sequence(pd_convolution_result,
                                    pd_symbols_frequence_part_of_one,
                                    pd_convolution_buffer,
                                    lenght_sequence);
    int index_maximum;
    correlation_research(pd_convolution_result, lenght_sequence, p_max_to_middle_others_relation,
                         p_sigma_middle_level, &index_maximum);

//    double maximum = -1.0;
//    int index_maximum = -1;
//    for(int symbol_count = 0; symbol_count < lenght_sequence; ++symbol_count) {
//        if (maximum < pd_convolution_result[symbol_count]) {
//            index_maximum = symbol_count;
//            maximum = pd_convolution_result[symbol_count];
//        }
//    }
//    double middle_level = 0.0;
//    for(int symbol_count = 0; symbol_count < lenght_sequence; ++symbol_count) {
//        if (index_maximum != symbol_count) {
//            middle_level += pd_convolution_result[symbol_count];
//        }
//    }
//    middle_level /= (double)(lenght_sequence-1);

//    *p_sigma_middle_level = 0.0;
//    for(int symbol_count = 0; symbol_count < lenght_sequence; ++symbol_count) {
//        if (index_maximum != symbol_count) {
//            *p_sigma_middle_level += fabs(pd_convolution_result[symbol_count] - middle_level);
//        }
//    }

//    *p_sigma_middle_level /= (double)(lenght_sequence-1);
//    *p_max_to_middle_others_relation = maximum / middle_level;
}
void correlation_research(double * pd_sequence,
                          int lenght_sequence,
                          double * p_max_to_middle_others_relation,
                          double * p_sigma_middle_level,
                          int * p_index_maximum)
{
    double maximum = -1.0;
    int index_maximum = -1;
    for(int symbol_count = 0; symbol_count < lenght_sequence; ++symbol_count) {
        if (maximum < pd_sequence[symbol_count]) {
            index_maximum = symbol_count;
            maximum = pd_sequence[symbol_count];
        }
    }
    //printf("maximum=pd_sequence[%d]=%f\n", index_maximum, pd_sequence[index_maximum]);
    *p_index_maximum = index_maximum;
    double middle_level = 0.0;
    for(int symbol_count = 0; symbol_count < lenght_sequence; ++symbol_count) {
        if (index_maximum != symbol_count) {
            middle_level += pd_sequence[symbol_count];
        }
    }
    middle_level /= (double)(lenght_sequence-1);

    *p_sigma_middle_level = 0.0;
    for(int symbol_count = 0; symbol_count < lenght_sequence; ++symbol_count) {
        if (index_maximum != symbol_count) {
            *p_sigma_middle_level += fabs(pd_sequence[symbol_count] - middle_level);
        }
    }

    *p_sigma_middle_level /= (double)(lenght_sequence-1);
    *p_max_to_middle_others_relation = maximum / middle_level;
}
//===========================================================================================================
