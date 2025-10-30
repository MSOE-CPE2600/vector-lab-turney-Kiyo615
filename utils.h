/****************************************
 * Filename: utils.h
 * Description: lab 5 Ultility functions header
 * Author: Carson Schur
 * Date: 10/8/25
 * compile: make file
****************************************/
#ifndef UTILS_H
#define UTILS_H

#include "vector.h"

/**
 * @brief Dump the workspace contents to stdout for debugging.
 * @param[in] tag Optional label printed in the header.
 * @return void
 */
void dump_workspace(const char *tag);


/**
 * @brief Tokenize an input line into space/comma-separated tokens.
 * @param[in,out] user_input Line to split; modified in place (NULs inserted).
 * @param[out] tokens       Output array of token pointers.
 * @param[in] maxtokens     Maximum number of tokens to store.
 * @return Number of tokens written to @p tokens.
 */
int tokenize_input(char *user_input, char **tokens, int maxtokens);

/**
 * @brief Clear all stored vectors from the workspace.
 * @return void
 */
void clear_mem();

int load_workspace(char **tokens);

int save_workspace(char **tokens);

/**
 * @brief Check whether a token is an operator and output it.
 * @param[in]  token   C-string to test ("*", "+", or "-").
 * @param[out] operand Receives the operator character if recognized.
 * @return 1 if token is an operator and @p operand is set; otherwise 0.
 */
int math_needed(char *token, char* operand);

/**
 * @brief Print one vector (if name in tokens[0]) or list all stored vectors.
 * @param[in] tokens Token array; if non-NULL, tokens[0] is the vector name.
 * @return void
 */
void print_workspace(char **tokens);

/**
 * @brief Evaluate a vector expression and assign the result to dst.
 * @param[in] tokens Token array: [0]=dst, [1]="=", [2]=lhs, [3]=op, [4]=rhs.
 * @param[in] op     Operator: '+', '-', or '*'.
 * @return Index of the stored vector on success; -1 on error.
 */
int do_math(char **tokens, char op);

/**
 * @brief Create or update a named 3D vector in the workspace.
 * @param[in] name Null-terminated vector name.
 * @param[in] x    X component.
 * @param[in] y    Y component.
 * @param[in] z    Z component.
 * @return Index of the stored vector, or -1 if no free slot.
 */
int assign_vector(const char *name, double x, double y, double z);

/**
 * @brief Print an error message and optionally echo tokens.
 * @param[in] tokens     Token array to echo; may be NULL.
 * @param[in] max_tokens Number of tokens to echo from @p tokens.
 * @param[in] message    Error message; if NULL, uses "Invalid input".
 * @return void
 */
void invalid_input(char **tokens, int max_tokens, char *message);

/**
 * @brief Find a vector by name in the workspace.
 * @param[in] name Null-terminated name to search for.
 * @return Index of the matching entry, or -1 if not found.
 */
int in_workspace(const char *name);

/**
 * @brief Check if tokens[lo..hi] are all numbers.
 * @param[in] tokens Token array. Not NULL.
 * @param[in] lo Start index inclusive.
 * @param[in] hi End index inclusive.
 * @return 1 if every token parses as a pure number, else 0.
 */
int is_numerical(char **tokens, int lo, int hi);

/**
 * @brief Check if tokens represent a complex number "a +/- j b".
 * @param[in] tokens Token array; expects at least 4 tokens: a, +/-, j, b.
 * @return 1 if format matches and a,b are numeric; otherwise 0.
 */
int is_complex(char **tokens);

/**
 * @brief Check if any workspace slot is in use.
 * @return 1 if at least one entry is used; otherwise 0.
 */
int any_used(void);

/**
 * @brief Get the index of the next used workspace slot at or after prev_idx.
 * @param[in] prev_idx Starting index to search from.
 * @return Index of the next used slot, or -1 if none.
 */
int next_used(int prev_idx);

/**
 * @brief Print the program help text to stdout.
 * @return void
 */
void print_help(void);

#endif


