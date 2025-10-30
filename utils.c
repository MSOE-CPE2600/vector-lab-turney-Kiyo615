/****************************************
 * Filename: utils.c
 * Description: lab 5 Ultility functions
 * Author: Carson Schur
 * Date: 10/8/25
 * compile: make file
 ****************************************/
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include "utils.h"
#include "vector.h"

// this is for debugging    
void dump_workspace(const char *tag) {
    if (tag && *tag) printf("---- workspace dump (%s) ----\n", tag);
    else             printf("---- workspace dump ----\n");

    for (int i = 0; i < mem_size; ++i) { 
        const vector *v = &workspace[i];
        if (v->used) {
            printf("[%02d] used=1 name='%s'  x=%g  y=%g  z=%g\n",
                   i, v->name, v->xval, v->yval, v->zval);
        } else {
            printf("[%02d] used=0\n", i);
        }
    }
    printf("------------------------------\n");
}


 
int tokenize_input(char *user_input, char **tokens, int maxtokens) {
    int count = 0;
    user_input[strcspn(user_input, "\n")] = '\0';
    char *token = strtok(user_input, " ,");
    while (token != NULL && count < maxtokens) {  
        tokens[count] = token;
        token = strtok(NULL, " ,");
        count++;
    }
    if (count < maxtokens) tokens[count] = NULL; 
    return count;
}

void clear_mem(){ // clears memory
    int idx = 1;
    while (workspace[idx].used == 1){
        workspace[idx].used = 0;
        idx++;
    }
    printf("        Memory cleared\n"); 
}

int load_workspace(char **tokens){
    // assume there are only two tokens
    FILE *fp;
    //make sure the file name ends in csv
    fp = fopen(tokens[1],"r");
    if (!fp){
        invalid_input(tokens,2,"Error opening file");
        return 1;
    }

    char line[256];
    // make sure work book is formatted right 
    // set workbook to workspace 
    int errors = 0;
    while (fgets(line, sizeof line, fp)) {
        char name[32];
        int x, y, z;
        if (sscanf(line, " %31[^,], %d , %d , %d", name, &x, &y, &z) == 4) {
            int rtn = assign_vector(name,x,y,z);
            if (rtn == -1){
                return 1;
            }
        }else if(errors == 0){
            errors++;
            invalid_input(tokens,2,"One or more vectors are bad in file:");
        }
    }
    fclose(fp);
    return 0;
}

int save_workspace(char **tokens){
    FILE *fp = fopen(tokens[1], "w");
    if (!fp) {
        invalid_input(tokens, 2, "Error opening file for write");
        return 1;
    }

    for (int i = 0; i < mem_size; i++) {
        if (!workspace[i].used) continue;
        if (fprintf(fp, "%s,%.15g,%.15g,%.15g\n",
                    workspace[i].name, workspace[i].xval, workspace[i].yval, workspace[i].zval) < 0) {
            fclose(fp);
            invalid_input(tokens, 2, "Write error");
            return 1;
        }
    }
    if (fclose(fp) != 0) {
        invalid_input(tokens, 2, "Close error (disk full?)");
        return 1;
    }
    printf("        Wrote workspace to \" %s \"\n",tokens[1]);
    return 0;
}

void print_workspace(char **tokens){
    if (tokens != NULL) {
        int idx = in_workspace(tokens[0]);
        printf("        %s = %f, %f, %f\n",
            workspace[idx].name,
            workspace[idx].xval, workspace[idx].yval, workspace[idx].zval); 
    }else if (any_used()){
        int idx = 0;
        while ((idx = next_used(idx)) != -1){
            printf("        %s = %f, %f, %f\n",
                workspace[idx].name,
                 workspace[idx].xval, workspace[idx].yval, workspace[idx].zval);
            idx++;
        }
    }else{
        invalid_input(tokens, 132/2 ,"No variables in the current workspace.");
    }
}

int math_needed(char *token, char* operand){
    if(!strcmp(token,"*")){
        // hey we doing multiplication
        *operand = '*';
        return 1;
    }else if (!strcmp(token,"+")){
        // hey we doing addition
        *operand = '+';
        return 1;
    }else if (!strcmp(token,"-")){
        // hey we doing subtraction
        *operand = '-';
        return 1;
    }
    return 0;
}

int do_math(char **tokens, char op){
    // tokens: [0]=dst, [1]="=", [2]=lhs, [3]=op, [4]=rhs
    char *dst = tokens[0];
    char *lhs = tokens[2];
    char *rhs = tokens[4];

    double ax=0, ay=0, az=0;  
    double bx=0, by=0, bz=0;  
    double rx=0, ry=0, rz=0;  
    double scalar=0;
    
    int lhs_is_num = is_numerical(tokens, 2, 2);
    int rhs_is_num = is_numerical(tokens, 4, 4);

    //printf("operation: %c\n", op);

    switch (op) {
        case '+':
        case '-': {
            if (lhs_is_num || rhs_is_num) {
                invalid_input(tokens, 5, "Add/Sub requires two vectors.");
                return -1;
            }
            int il = in_workspace(lhs);
            int ir = in_workspace(rhs);
            if (il < 0 || ir < 0) {
                invalid_input(tokens, 5, "Undefined variable in calculation");
                return -1;
            }
            ax = workspace[il].xval; ay = workspace[il].yval; az = workspace[il].zval;
            bx = workspace[ir].xval; by = workspace[ir].yval; bz = workspace[ir].zval;

            if (op == '+') {
                //printf("we doing addition\n");
                rx = ax + bx; ry = ay + by; rz = az + bz;
            } else {
                //printf("we doing subtraction\n");
                rx = ax - bx; ry = ay - by; rz = az - bz;
            }
            break;
        }

        case '*': {
            // exactly one side must be numeric for scalar multiplication
            if (lhs_is_num == rhs_is_num) {  // both numeric or both vectors
                invalid_input(tokens, 5, "Scalar multiplication requires one number and one vector.");
                return -1;
            }

            if (lhs_is_num) {
                // scalar * vector(rhs)
                if (sscanf(lhs, "%lf", &scalar) != 1) {
                    invalid_input(tokens, 5, "Bad scalar on LHS.");
                    return -1;
                }
                int ir = in_workspace(rhs);
                if (ir < 0) {
                    invalid_input(tokens, 5, "Undefined variable on RHS.");
                    return -1;
                }
                bx = workspace[ir].xval; by = workspace[ir].yval; bz = workspace[ir].zval;
                rx = scalar * bx; ry = scalar * by; rz = scalar * bz;
            } else {
                // vector(lhs) * scalar
                if (sscanf(rhs, "%lf", &scalar) != 1) {
                    invalid_input(tokens, 5, "Bad scalar on RHS.");
                    return -1;
                }
                int il = in_workspace(lhs);
                if (il < 0) {
                    invalid_input(tokens, 5, "Undefined variable on LHS.");
                    return -1;
                }
                ax = workspace[il].xval; ay = workspace[il].yval; az = workspace[il].zval;
                rx = ax * scalar; ry = ay * scalar; rz = az * scalar;
            }
            //printf("we doing multiplication\n");
            break;
        }

        default:
            invalid_input(tokens, 5, "Operand not supported.");
            return -1;
    }

    return assign_vector(dst, rx, ry, rz);
}

int assign_vector(const char *name, double x, double y, double z){
    int idx = in_workspace(name);
    if (idx < 0 ) {
        //assign new memory
        mem_size = mem_size + 1;
        vector *temp = realloc(workspace, mem_size * sizeof(vector));
        if (temp == NULL){
            return -1; // full
        }else{
            workspace = temp;
            temp = NULL;
            idx = (int)mem_size-1;
        }
        strcpy(workspace[idx].name, name);
        workspace[idx].used = 1;
    }
    workspace[idx].xval = x;
    workspace[idx].yval = y;
    workspace[idx].zval = z;

    printf("        %s = %f, %f, %f\n",
           workspace[idx].name,
           workspace[idx].xval, workspace[idx].yval, workspace[idx].zval);

    return idx;  
}

void invalid_input(char **tokens, int max_tokens, char *message) {
    if (!message) message = "Invalid input";

    // Always print the message
    printf("%s", message);

    // If no tokens to echo, just end the line
    if (!tokens || max_tokens <= 0) {
        printf("\n");
        return;
    }

    // Echo provided tokens safely
    printf("\n    \"");
    for (int i = 0; i < max_tokens; ++i) {
        if (tokens[i]) {
            printf("%s", tokens[i]);
            if (i < max_tokens - 1) printf(" ");
        }
    }
    printf("\"\n");
}

int in_workspace(const char *name) {
     for (int i = 0; i < mem_size; ++i) {  
        if (workspace[i].used && strcmp(workspace[i].name, name) == 0)
            return i;
    }
    return -1;
}

int is_numerical(char **tokens, int lo, int hi) {
    if (!tokens || lo < 0 || hi < lo) return 0;

    for (int i = lo; i <= hi; i++) {
        if (tokens[i] == NULL) return 0;  // missing token inside range

        double tmp;
        char extra;
        int ok = sscanf(tokens[i], " %lf %c", &tmp, &extra);
        if (ok != 1) return 0;            // not a pure number
    }
    return 1;
}

int any_used(void) {
    for (int i = 0; i < mem_size; ++i) 
        if (workspace[i].used == 1) return 1;
    return 0;
}

int next_used(int prev_idx) {
    for (int i = prev_idx; i < mem_size; ++i) 
        if (workspace[i].used) return i;
    return -1; // none
}

void print_help(void) {
    printf(
        "calc - a tiny vector REPL (Matlab style)\n"
        "\n"
        "Usage:\n"
        "  ./calc           Start interactive prompt\n"
        "  ./calc -h        Show this help and exit\n"
        "\n"
        "Overview:\n"
        "  Store up to 10 named 3D vectors and do basic math.\n"
        "\n"
        "Commands:\n"
        "  quit             Exit the program\n"
        "  clear            Remove all stored vectors\n"
        "  list             Show all stored vectors\n"
        "  <name>           Show the value of a single vector (if it exists)\n"
        "\n"
        "Assignment:\n"
        "  name = x y z\n"
        "  name = x, y, z\n"
        "  Notes:\n"
        "    - Spaces are required around =\n"
        "    - Commas between numbers are optional\n"
        "    - If z is omitted, it defaults to 0.0 (optional behavior)\n"
        "\n"
        "Operations:\n"
        "  a + b            Vector addition\n"
        "  a - b            Vector subtraction\n"
        "  a * k            Scalar multiplication (k is a float)\n"
        "  k * a            Scalar multiplication\n"
        "  result = a + b   Do the operation and store the result\n"
        "  result = a - b\n"
        "  result = a * k   (Spaces required around = and operators)\n"
        "\n"
        "Display format:\n"
        "  Values are printed as: name = x, y, z\n"
        "\n"
        "Errors:\n"
        "  Referencing a non-existent vector or giving a bad expression prints an error.\n"
        "\n"
        "Examples:\n"
        "  a = 1,2,3\n"
        "  b = 4 5 6\n"
        "  c = a + b\n"
        "  a = 2.5 * a\n"
        "  sum = a + b\n"
        "  list\n"
        "\n"
    );
}