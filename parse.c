/****************************************
 * Filename: parse.c
 * Description: lab 5 Ultility functions
 * Author: Carson Schur
 * Date: 10/8/25
 * compile: make file
 ****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "vector.h" 

int parse_input(int num_tokens, char **tokens){
    char operation[1];

    if (num_tokens == 1){ //single commands
        //check single tokens
        if (!strcmp(tokens[0], "clear")){
            clear_mem();
        }else if (!strcmp(tokens[0], "quit")){
            printf("Closing program...\n");
            return 1;
        }else if (!strcmp(tokens[0], "list")){
            print_workspace(NULL);
        }else if (in_workspace(tokens[0]) != -1){
            print_workspace(tokens); 
        }else{
            invalid_input(tokens, 1 ,"unknown command: ");
        }
    }else if ((num_tokens <= 5 && num_tokens >= 3) && !strcmp(tokens[1], "=")){ //assigmnent - vector
        // assume all vectors are 3-d
        char operation;
        if (math_needed(tokens, num_tokens, &operation)){
            // we doing some math 
            do_math(tokens, operation);
        }else if (is_numerical_range(tokens, 2, 4) || is_numerical_range(tokens, 2, 3) || is_numerical_range(tokens, 2, 2)){
            // we doing some assignment (numerical only)
            double x = 0, y = 0, z = 0;
            sscanf(tokens[2], "%lf", &x);
            if (num_tokens >= 4) sscanf(tokens[3], "%lf", &y);
            if (num_tokens >= 5) sscanf(tokens[4], "%lf", &z);
            int rtn = assign_vector(tokens[0], x, y, z);
            if(rtn == -1){
                invalid_input(tokens, num_tokens, "Too many vectors! Please clear and try again.");
            }
        }else{
            invalid_input(tokens, num_tokens, "Bad vector component");
        }
    }else if (num_tokens == 0){
    }else{ // invalid input
        invalid_input(tokens, 132/2 ,"Invalid calculation or operation: ");  
    }
    return 0;
}