/* 
* Filename: vector.h
* description: struct for work space
* Author: Carson Schur
* Date: 9/15/25
*/

#ifndef VECTOR_H
#define VECTOR_H

#define MAX_VECS 4

typedef struct {
    char name[16];
    int used;
    double xval;
    double yval;
    double zval;
}vector;

extern vector workspace[MAX_VECS];

#endif