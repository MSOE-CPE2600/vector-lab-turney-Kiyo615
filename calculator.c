/****************************************
 * Filename: calculator.c
 * Description: lab 5 calculator
 * Author: Carson schur
 * Date: 10/8/25
 * compile: make file
 ****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "parse.h"
#include "vector.h"

#define INPUT_SIZE 132

vector workspace[MAX_VECS] = {0};



int main (int argc, char* argv[]){
    
    //num of args 
    if (argc == 2 && !strcmp(argv[1], "-h")){ //print help text
        print_help();

    }else if(argc == 1){ //run program 

    while (1){
        char user_input[INPUT_SIZE];
        char *tokens[INPUT_SIZE/2] = {0};
        // new line
        printf(">> ");
        fgets(user_input, sizeof(user_input), stdin);

        int num_tokens = tokenize_input(user_input, tokens, sizeof(tokens));

        // for debugging
        /*for (int i = 0; i < num_tokens; i++) {
            printf("token[%d] = %s\n", i, tokens[i]);
        }*/

        if (parse_input(num_tokens, tokens)){
            exit(0);
        }
    }

    }else{ //invalid input
        printf("Please run the program or use '-h' for help\n");
    }
    
    return 0;
}
