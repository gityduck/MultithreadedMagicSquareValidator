/* Main program - handles file input, memory allocation, creating threads and printing the final report*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "validator.h"

int n;
int **matrix;
int *valid_row;
int *valid_col;
int valid_diags[2];
int valid_unique = 1;
int score = 0, magic_constant = 0;

pthread_mutex_t lock;

int main(int argc, char *argv[])
{
	/*create thread*/
    pthread_t t1, t2, t3, t4;
	pthread_mutex_init(&lock, NULL);

	if (argc != 2)
	{
		printf("Usage: %s <file name>.txt\n", argv[0]);
		return 1;
	}

	FILE *fptr = fopen(argv[1], "r");
	if (fptr == NULL)
	{
		printf("Error opening file\n");
		return 0;
	}
	fscanf(fptr, "%d", &n);
	
	magic_constant = (n*(n*n + 1))/2;
	int max_score = n + n + 2 + 1;

	/*allocate rows and cols*/
	matrix = malloc(n*sizeof(int*));
    for (int i = 0; i < n; i++)
    {
            matrix[i] = malloc(n*sizeof(int));
    }

	/*read the matrix*/
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(fptr, "%d", &matrix[i][j]);
        }
    }
    fclose(fptr);

	valid_row = malloc(n*sizeof(int));
	valid_col = malloc(n*sizeof(int));

	/*initialise*/
	for (int i = 0; i < n; i++)
	{
		valid_row[i] = 0;
		valid_col[i] = 0;
	}

	valid_diags[0] = 0;
	valid_diags[1] = 0;
	
	pthread_create(&t1, NULL, check_row, NULL);
	pthread_create(&t2, NULL, check_col, NULL);
	pthread_create(&t3, NULL, check_diags, NULL);
	pthread_create(&t4, NULL, check_unique, NULL);

	/*wait for thread*/
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);

	/*output report*/
	printf("\n");
	printf("--- Magic Square Report ---\n");

	/*for row*/
	int row_valid = 1;
	int flag = 0;
	printf("Rows: ");
	for (int i = 0; i < n; i++)
	{
		if (valid_row[i] == 0)
		{
			if (flag != 0)
			{
				printf(", ");
			}
			printf("Row %d", i + 1);
			row_valid = 0;
			flag = 1;
		}
	}
	if (row_valid)
	{
		printf("All Valid\n");
	}
	else
	{
		printf(" Invalid\n");
	}

	/*for col*/
	int Flag = 0;
	int col_valid = 1;
	printf("Cols: ");
	for (int j = 0; j < n; j++)
	{
		if (valid_col[j] == 0)
		{
			if (Flag != 0)
			{
				printf(", ");
			}
			printf("Col %d", j + 1);
			col_valid = 0;
			Flag = 1;
		}
	}
	if (col_valid)
	{
		printf("All Valid\n");
	}
	else
	{
		printf(" Invalid\n");
	}

	/*for diag*/
	printf("Diags: ");
	if (valid_diags[0] == 0 && valid_diags[1] == 0)
	{
		printf("Both Main and Secondary Diag Invalid\n");
	}
	else if (valid_diags[0] == 0)
	{
		printf("Main Diag Invalid\n");
	}
	else if (valid_diags[1] == 0)
	{
		printf("Secondary Diag Invalid\n");
	}
	else
	{
		printf("All Valid\n");
	}

	/*for uniqueness*/
	printf("Unique: ");
	if(valid_unique)
	{
		printf("Passed\n");
	}
	else
	{
		printf("Failed (Duplicates found)\n");
	}

	/*for final score*/
	printf("Final Score: %d / %d\n", score, max_score);
	if (score == max_score)
	{
		printf("RESULT: VALID MAGIC SQUARE\n");
	}
	else
	{
		printf("RESULT: INVALID MAGIC SQUARE\n");
	}

	/*free memory*/
	for (int i = 0; i < n; i++)
	{
		free(matrix[i]);
	}
	free(matrix);
	free(valid_row);
	free(valid_col);
	return 0;
}
