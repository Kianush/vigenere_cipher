#ifndef VIGENERE_ENCRYPT
#define VIGENERE_ENCRYPT
//===========================================================================================================
#include <stdio.h>
#include <ctype.h>
//===========================================================================================================
int vigenere_encrypt(const char * p_source_file, const char * p_dest_file);
int vigenere_decrypt(const char * p_source_file, const char * p_dest_file);
int vigenere_decrypt_with_guess_key(FILE * p_file_source, int lenght_guess_key);
int frequence_analiz(const char * p_source_file);
//===========================================================================================================
int set_data();
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
void correlation_research(double * pd_sequence,
                          int lenght_sequence,
                          double * p_max_to_middle_others_relation,
                          double * p_sigma_middle_level,
                          int * p_index_maximum);
int set_subarrays_symbols_frequence_into_crypted_text(FILE * p_file_source);
int set_subarrays_symbols_frequence_guess_key(FILE * p_file_source,
                                              double ** ppd_array_frequencies,
                                              int lenght_guess_key);
int convolution_research(int * p_index_mod_with_maximum, int * p_index_key_symbol);
int convolution_research_rested_key_symbols(double ** ppd_array_frequencies, int lenght_guess_key);
double ** set_key_search_data(int lenght_guess_key);
void free_key_search_data(double ** pp_data, int lenght_guess_key);
//===========================================================================================================
#endif
