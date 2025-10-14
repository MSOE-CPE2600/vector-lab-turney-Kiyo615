/****************************************
 * Filename: utils.h
 * Description: lab 5 Ultility functions header
 * Author: Carson Schur
 * Date: 10/8/25
 * compile: make file
****************************************/
#ifndef UTILS_H
#define UTILS_H


void dump_workspace(const char *tag);



int tokenize_input(char *user_input, char **tokens, int maxtokens);

int math_needed(char **tokens, int length_tokens, char* operand);

void clear_mem();

void print_workspace(char **tokens);

int do_math(char **tokens, char op);

int assign_vector(const char *name, double x, double y, double z);

void invalid_input(char **tokens, int max_tokens, char *message);

int in_workspace(const char *name);

int is_numerical_range(char **tokens, int lo, int hi);

int any_used(void);

int next_used(int prev_idx);

void print_help(void);

#endif