#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//===========================================================================================================
#include "vigenere.h"
//===========================================================================================================
void usage()
{
    printf("syntax : \n./vigenere_cipher --help or\
           \n./vigenere_cipher --decrypt_file [input_file] [output_file] or\
           \n./vigenere_cipher --encrypt_file [input_file] [output_file] or\
           \n./vigenere_cipher --analiz_file [input_file] or\
           \n./vigenere_cipher --test_algorithm [input_file]");
}
//===========================================================================================================
void error_parameters_count(const char * operation, const int correct_count_parametrs)
{
    printf("operatinon %s requires %d paramettrs", operation, correct_count_parametrs);
}
//===========================================================================================================
int main(int argc, char **argv)
{
    const int iCORRECT_COUNT_PARAMETRS_ENCRYPT_DECRYPT = 4,
              iCORRECT_COUNT_PARAMETRS_ANALIZ = 3,
              iCORRECT_COUNT_PARAMETRS_TEST = 3,
	      iSTANDART_COUNT = 2;
    const char * pcOPERATION_HELP = "--help";
    const char * pcOPERATION_ENCRYPT = "--encrypt_file";
    const char * pcOPERATION_DECRYPT = "--decrypt_file";
    const char * pcOPERATION_ANALIZ = "--analiz_file";
    const char * pcOPERATION_TEST = "--test_algorithm";
    const char * pcFILENAME_KEYFILE = "./keys/vigenere_key";
    int result_test_minimal_lenght = -1;
    int i_to_return = 0;
    if (set_data() < 0) {
        perror("error at allocate or set data");
        return EXIT_FAILURE;
    }
    if(get_key_from_file("./keys/vigenere_key") < 0) {
        printf("key for encrypt/decrypt not load from file %s\n", pcFILENAME_KEYFILE);
        printf("default key is used: ");
    }

    if(argc <= 1) {
        usage();
        free_data();
        i_to_return = EXIT_FAILURE;
        return i_to_return;
    }
    if(!strcmp(argv[1], pcOPERATION_HELP)) {
        usage();
        i_to_return = EXIT_SUCCESS;
    }
    else if(!strcmp(argv[1], pcOPERATION_DECRYPT)) {
        if(argc != iCORRECT_COUNT_PARAMETRS_ENCRYPT_DECRYPT) {
            error_parameters_count(pcOPERATION_DECRYPT, iCORRECT_COUNT_PARAMETRS_ENCRYPT_DECRYPT - iSTANDART_COUNT);
            i_to_return = EXIT_FAILURE;
        }
        else {
            if(vigenere_decrypt(argv[2], argv[3]) < 0) {
                fprintf(stderr, "vigenere_decrypt failed\n");
                i_to_return = EXIT_FAILURE;
            }
            else {
                printf("vigenere_decrypt created %s file\n", argv[3]);
                i_to_return = EXIT_SUCCESS;
            }
        }
    }
    else if (!strcmp(argv[1], pcOPERATION_ENCRYPT)) {
        if (argc != iCORRECT_COUNT_PARAMETRS_ENCRYPT_DECRYPT) {
           error_parameters_count(pcOPERATION_ENCRYPT, iCORRECT_COUNT_PARAMETRS_ENCRYPT_DECRYPT - iSTANDART_COUNT);
           i_to_return = EXIT_FAILURE;
        }
        else {
           if(vigenere_encrypt(argv[2], argv[3]) < 0) {
               fprintf(stderr, "vigenere_encrypt failed\n");
               i_to_return = EXIT_FAILURE;
           }
           else {
               printf("vigenere_encrypt created '%s' file\n", argv[3]);
               i_to_return = EXIT_SUCCESS;
           }
        }
    }
    else if (!strcmp(argv[1], pcOPERATION_ANALIZ)) {
        if (argc != iCORRECT_COUNT_PARAMETRS_ANALIZ) {
            error_parameters_count(pcOPERATION_ANALIZ, iCORRECT_COUNT_PARAMETRS_ANALIZ - iSTANDART_COUNT);
            i_to_return = EXIT_FAILURE;
        }
        else {
            if(frequence_analiz(argv[2]) < 0) {
                fprintf(stderr, "frequence_analiz failed\n");
                i_to_return = EXIT_FAILURE;
            }
            else {
               i_to_return = EXIT_SUCCESS;
            }
        }
    }
    else if (!strcmp(argv[1], pcOPERATION_TEST)) {
        if (argc != iCORRECT_COUNT_PARAMETRS_TEST) {
            error_parameters_count(pcOPERATION_TEST, iCORRECT_COUNT_PARAMETRS_TEST - iSTANDART_COUNT);
            i_to_return = EXIT_FAILURE;
        }
        else {
            if(test_minimal_text_file_lenght(argv[2], &result_test_minimal_lenght) < 0) {
                fprintf(stderr, "test_minimal_text_file_lenght failed\n");
                i_to_return = EXIT_FAILURE;
            }
            else {
                printf("result_test_minimal_lenght is: %d\n", result_test_minimal_lenght);
                i_to_return = EXIT_SUCCESS;
            }
        }
    }
    else {
        printf("Unknown parameter: '%s'. Type ./vigenere_cipher --help for help.\n", argv[1]);
        i_to_return = EXIT_FAILURE;
    }
    free_data();
    return i_to_return;
 }
