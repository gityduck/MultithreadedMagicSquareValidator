/* Contains functions for rows, columns, diagonals  and uniqueness validations */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "validator.h"

void* check_row(void* arg)
{
	for (int i = 0; i < n; i++)
	{
		int sum = 0;
		for (int j = 0; j < n; j++)
		{
			sum += matrix[i][j];
		}

		sleep(1);

		if (sum == magic_constant)
		{
			valid_row[i] = 1;
			pthread_mutex_lock(&lock);
			score++;
			pthread_mutex_unlock(&lock);
		}
		else
		{
			valid_row[i] = 0;
			pthread_mutex_lock(&lock);
			score--;
			if (score < 0)
			{
				score = 0;
			}
			pthread_mutex_unlock(&lock);
		}
	}
	printf("Thread ID-%ld: Row checks completed.\n", pthread_self());
	return NULL;
}

void* check_col(void* arg)
{
	for (int j = 0; j < n; j++)
	{
		int sum = 0;
		for (int i = 0; i < n; i++)
		{
			sum += matrix[i][j];
		}

		sleep(1);

		if (sum == magic_constant)
		{
			valid_col[j] = 1;
			pthread_mutex_lock(&lock);
			score++;
			pthread_mutex_unlock(&lock);
		}
		else
		{
			valid_col[j] = 0;
			pthread_mutex_lock(&lock);
			score--;
			if (score < 0)
			{
				score = 0;
			}
			pthread_mutex_unlock(&lock);
		}
	}
	printf("Thread ID-%ld: Column checks completed.\n", pthread_self());
	return NULL;
}

void* check_diags(void* arg)
{
	int sum_main = 0;
	int sum_secondary = 0;

	for (int i = 0; i < n; i++)
	{
		sum_main += matrix[i][i];
		sum_secondary += matrix[i][n-1-i];
	}

	sleep(1);

	/*for main diag*/
	if (sum_main == magic_constant)
	{
		valid_diags[0] = 1;
		pthread_mutex_lock(&lock);
		score++;
		pthread_mutex_unlock(&lock);
	}
	else
	{
		valid_diags[0] = 0;
		pthread_mutex_lock(&lock);
		score--;
		if (score < 0)
		{
			score = 0;
		}
		pthread_mutex_unlock(&lock);
	}

	/*for sec diag*/
	if (sum_secondary == magic_constant)
	{
		valid_diags[1] = 1;
		pthread_mutex_lock(&lock);
		score++;
		pthread_mutex_unlock(&lock);
	}
	else
	{
		valid_diags[1] = 0;
		pthread_mutex_lock(&lock);
		score--;
		if (score < 0)
		{
			score = 0;
		}
		pthread_mutex_unlock(&lock);
	}
	printf("Thread ID-%ld: Diagonal checks completed.\n", pthread_self());
	return NULL;
}

void* check_unique(void* arg)
{
	int grid_size = n*n;
	int *encountered = (int*)calloc(grid_size + 1, sizeof(int));
	int is_unique = 1;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int number = matrix[i][j];
			/*check if number is out of range*/
			if (number < 1 || number > grid_size)
			{
				is_unique = 0;
			}
			else
			{
				/*check if number have been encountered before*/
				if (encountered[number] == 0)
				{
					encountered[number] = 1;
				}
				else
				{
					/*duplicate number found*/
					is_unique = 0;
				}
			}
		}
	}

	sleep(1);

	if (is_unique)
	{
		valid_unique = 1;
		pthread_mutex_lock(&lock);
		score++;
		pthread_mutex_unlock(&lock);
	}
	else
	{
		valid_unique = 0;
		pthread_mutex_lock(&lock);
		score--;
		if (score < 0)
		{
			score = 0;
		}
		pthread_mutex_unlock(&lock);
	}
	free(encountered);
	printf("Thread ID-%ld: Uniqueness check completed.\n", pthread_self());
	return NULL;
}