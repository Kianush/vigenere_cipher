#include <stdio.h>
#include <string.h>
//===========================================================================================================
#include "vigenere_encrypt.h"
//===========================================================================================================
void usage() {
    printf("syntax : \n\t./vigenere_cipher --help \
           or\n\t./vigenere_cipher --decrypt_file [input_file] [output_file] \
           or\n\t./vigenere_cipher --encrypt_file [input_file] [output_file]");
}
//===========================================================================================================
void error_parameters_count(const char * operation, const int correct_count_parametrs) {
    printf("operatinon %s requires %d paramettrs", operation, correct_count_parametrs);
}
//===========================================================================================================
int main(int argc, char **argv) {
    //test_convolution_with_basic_sequence();

    const int iCORRECT_COUNT_PARAMETRS = 4;
    const char * pcOPERATION_ENCRYPT = "--encrypt_file";
    const char * pcOPERATION_DECRYPT = "--decrypt_file";
    const char * pcOPERATION_ANALIZ = "--analiz_file";
    set_data();

    //autocorrelation_research(NULL);

    if(argc <= 1) {
        usage();
        return 0;
    }
    if(!strcmp(argv[1], "--help")) {
        printf("You requested help message.\n");
    }
    else if(!strcmp(argv[1], pcOPERATION_DECRYPT)) {
        if(argc != iCORRECT_COUNT_PARAMETRS) {
            error_parameters_count(pcOPERATION_DECRYPT, iCORRECT_COUNT_PARAMETRS);
        }
        else {
            vigenere_decrypt(argv[2], argv[3]);
        }
    }
    else if (!strcmp(argv[1], pcOPERATION_ENCRYPT)) {
        if (argc != iCORRECT_COUNT_PARAMETRS) {
           error_parameters_count(pcOPERATION_ENCRYPT, iCORRECT_COUNT_PARAMETRS);
        }
        else {
           vigenere_encrypt(argv[2], argv[3]);
        }
    }
    else if (!strcmp(argv[1], pcOPERATION_ANALIZ)) {
        if (argc != iCORRECT_COUNT_PARAMETRS) {
           error_parameters_count(pcOPERATION_ENCRYPT, iCORRECT_COUNT_PARAMETRS);
        }
        else {
           frequence_analiz(argv[2], argv[3]);
        }
    }
    else {
        printf("Unknown parameter: '%s'. Type ./vigenere_cipher --help for help.\n", argv[1]);
    }
    free_data();
    return 0;
 }
