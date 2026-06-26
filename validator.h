/* Header file for defining shared global variables and function prototypes*/

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <pthread.h>

/*shared global variables*/
extern int n;
extern int **matrix;
extern int magic_constant;

extern int *valid_row;
extern int *valid_col;
extern int valid_diags[2];
extern int valid_unique;
extern int score;

extern pthread_mutex_t lock;

void* check_row(void* arg);
void* check_col(void* arg);
void* check_diags(void* arg);
void* check_unique(void* arg);

#endif
