#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//===========================================================================================================
#include "vigenere_encrypt.h"
//===========================================================================================================
void usage()
{
    printf("syntax : \n\t./vigenere_cipher --help \
           or\n\t./vigenere_cipher --decrypt_file [input_file] [output_file] \
           or\n\t./vigenere_cipher --encrypt_file [input_file] [output_file]");
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
              iCORRECT_COUNT_PARAMETRS_ANALIZ = 3;
    const char * pcOPERATION_ENCRYPT = "--encrypt_file";
    const char * pcOPERATION_DECRYPT = "--decrypt_file";
    const char * pcOPERATION_ANALIZ = "--analiz_file";
    if (set_data() < 0) {
        perror("error at allocate or set data");
        return EXIT_FAILURE;
    }

    if(argc <= 1) {
        usage();
        return EXIT_FAILURE;
    }
    if(!strcmp(argv[1], "--help")) {
        printf("You requested help message.\n");
    }
    else if(!strcmp(argv[1], pcOPERATION_DECRYPT)) {
        if(argc != iCORRECT_COUNT_PARAMETRS_ENCRYPT_DECRYPT) {
            error_parameters_count(pcOPERATION_DECRYPT, iCORRECT_COUNT_PARAMETRS_ENCRYPT_DECRYPT);
        }
        else {
            vigenere_decrypt(argv[2], argv[3]);
        }
    }
    else if (!strcmp(argv[1], pcOPERATION_ENCRYPT)) {
        if (argc != iCORRECT_COUNT_PARAMETRS_ENCRYPT_DECRYPT) {
           error_parameters_count(pcOPERATION_ENCRYPT, iCORRECT_COUNT_PARAMETRS_ENCRYPT_DECRYPT);
        }
        else {
           if(vigenere_encrypt(argv[2], argv[3]) < 0) {
               fprintf(stderr, "vigenere_encrypt failed\n");
           }
        }
    }
    else if (!strcmp(argv[1], pcOPERATION_ANALIZ)) {
        if (argc != iCORRECT_COUNT_PARAMETRS_ANALIZ) {
            error_parameters_count(pcOPERATION_ANALIZ, iCORRECT_COUNT_PARAMETRS_ANALIZ);

        }
        else {
            if(frequence_analiz(argv[2]) < 0) {
                fprintf(stderr, "frequence_analiz failed\n");
            }
        }
    }
    else {
        printf("Unknown parameter: '%s'. Type ./vigenere_cipher --help for help.\n", argv[1]);
    }
    free_data();
    return EXIT_SUCCESS;
 }
