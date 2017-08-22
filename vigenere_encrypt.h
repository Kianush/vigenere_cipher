#ifndef VIGENERE_ENCRYPT
#define VIGENERE_ENCRYPT
//===========================================================================================================
#include <stdio.h>
#include <ctype.h>
//===========================================================================================================
void vigenere_encrypt(const char * p_source_file, const char * p_dest_file);
void vigenere_decrypt(const char * p_source_file, const char * p_dest_file);
void frequence_analiz(const char * p_source_file, const char * p_dest_file);
//===========================================================================================================
void set_data();
void free_data();
//===========================================================================================================
void set_english_lower_character_alphabet(char *p_alphabet);
void set_english_symbols_typical_frequence(double * p_array);
void set_vigenere_square(char ** p_vigenere_square, char *p_alphabet,  const int alphabet_power);
void set_array_numbers_of_charasters(unsigned int * p_array, char * p_alphabet, const int alphabet_power);
char get_vigenere_encrypted_char(const char ch_source, const int position,
                                 const char * p_key, const int size_of_key);
char get_vigenere_decrypted_char(const char ch_source, const int position,
                                 const char * p_key, const int size_of_key);
void reset_array_symbols_frequence_into_crypted_text();
void convolution_with_basic_sequence(double * p_researched_sequence, double * p_basic_sequence,
                                     double *p_temp, const int size);
void test_convolution_with_basic_sequence();
void autocorrelation_research(double * pd_convolution_result,
                              double * pd_basic_sequence,
                              double * pd_convolution_buffer,
                              int lenght_sequence,
                              double * p_max_to_middle_others_relation,
                              double * p_sigma_middle_level);
void correlation_research(double * pd_sequence,
                          int lenght_sequence,
                          double * p_max_to_middle_others_relation,
                          double * p_sigma_middle_level,
                          int * p_index_maximum);
//===========================================================================================================
#endif
