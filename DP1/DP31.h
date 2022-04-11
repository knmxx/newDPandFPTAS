#pragma once
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "job.h"
using namespace std;
struct DP3Path
{
public:
	int before_A;
	int before_B;
	int before_C;
	int m;
};
DP3Path **** dp3path_table;
int dp3comple_table[3];
double dp3_running_time = -1;
int **** dp3_table;
int dp3result = -1;
Job dp3job_array[MAX_N];
void new_dptable3()
{
	dp3_table = new int***[current_config.n + 1];
	dp3path_table = new DP3Path ***[current_config.n + 1];
	for (int i = 0; i < current_config.n + 1; ++i)
	{
		dp3_table[i] = new int**[current_config.D + 1];
		dp3path_table[i] = new DP3Path **[current_config.D + 1];
	}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < current_config.D + 1; j++)
		{
			dp3_table[i][j] = new int*[current_config.D + 1];
			dp3path_table[i][j] = new DP3Path*[current_config.D + 1];
		}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < current_config.D + 1; j++)
			for (int k = 0; k < current_config.D + 1; k++) 
			{
				dp3_table[i][j][k] = new int[current_config.D + 1];
 				dp3path_table[i][j][k] = new DP3Path[current_config.D + 1];
			}
}
void init_dptable3()
{
	memset(dp3comple_table, 0, sizeof(dp3comple_table));
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= current_config.D; ++j)
		{
			for (int k = 0; k <= current_config.D; ++k)
			{
				for (int x = 0; x <= current_config.D; ++x) 
				{ 
					dp3_table[i][j][k][x] = 0;
					dp3path_table[i][j][k][x].before_A = 0;
					dp3path_table[i][j][k][x].before_B = 0;
					dp3path_table[i][j][k][x].before_C = 0;
					dp3path_table[i][j][k][x].m = 0;
				}
			}
		}
	}
}
void print_dp3pathtable()
{
	dp3result = 0;
	DP3Path current_job = dp3path_table[current_config.n][current_config.D][current_config.D][current_config.D];
	for (int i = current_config.n; i > 0; --i)
	{

		if (current_job.m == 1)
		{
			dp3comple_table[0] += dp3job_array[i - 1].proc_time;
			dp3result += job_late_work(dp3job_array[i - 1], dp3comple_table[0]);
		}
		if (current_job.m == 2)
		{
			dp3comple_table[1] += dp3job_array[i - 1].proc_time;
			dp3result += job_late_work(dp3job_array[i - 1], dp3comple_table[1]);
		}
		if (current_job.m == 3)
		{
			dp3comple_table[2] += dp3job_array[i - 1].proc_time;
			dp3result += job_late_work(dp3job_array[i - 1], dp3comple_table[2]);
		}
		current_job = dp3path_table[i - 1][current_job.before_A][current_job.before_B][current_job.before_C];
	}
}
void printf_dptable3()
{
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= current_config.D; ++j)
		{
			for (int k = 0; k <= current_config.D; ++k)
			{
				for (int x = 0; x <= current_config.D; ++x)
				{
					printf("f(%d,%d,%d,%d)=%d ", i, j, k, x, dp3_table[i][j][k][x]);
				}
				printf("\n");
			}
			printf("\n");
		}
		printf("\n");
	}
}
void free_dptable3()
{
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= current_config.D; ++j)
		{
			for (int k = 0; k <= current_config.D; ++k)
			{
				delete[] dp3_table[i][j][k];
				delete[] dp3path_table[i][j][k];
			}
			delete[] dp3_table[i][j];
			delete[] dp3path_table[i][j];
		}
		delete[] dp3_table[i];
		delete[] dp3path_table[i];
	}
	delete[] dp3_table;
	delete[] dp3path_table;
}
int dp31()
{
	memcpy(dp3job_array, job_set, sizeof(job_set));
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);
	new_dptable3();
	init_dptable3();

	for (int j = 1; j <= current_config.n; ++j)
	{
		for (int A = 0; A <= current_config.D; ++A)
		{
			for (int B = 0; B <= current_config.D; ++B)
			{
				for (int C = 0; C <= current_config.D; ++C)
				{
					int ra = max(A - job_set[j - 1].proc_time, 0);
					int rb = max(B - job_set[j - 1].proc_time, 0);
					int rc = max(C - job_set[j - 1].proc_time, 0);
					int ya = max(job_set[j - 1].proc_time - A, 0);
					int yb = max(job_set[j - 1].proc_time - B, 0);
					int yc = max(job_set[j - 1].proc_time - C, 0);
					int fa = dp3_table[j - 1][ra][B][C] + ya;
					int fb = dp3_table[j - 1][A][rb][C] + yb;
					int fc = dp3_table[j - 1][A][B][rc] + yc;
					
					if (fa <= fb && fa <= fc) 
					{
						dp3_table[j][A][B][C] = fa;
						dp3path_table[j][A][B][C].before_A = ra;
						dp3path_table[j][A][B][C].before_B = B;
						dp3path_table[j][A][B][C].before_C = C;
						dp3path_table[j][A][B][C].m = 1;
					}
					else if (fb <= fa && fb <= fc)
					{
						dp3_table[j][A][B][C] = fb;
						dp3path_table[j][A][B][C].before_A = A;
						dp3path_table[j][A][B][C].before_B = rb;
						dp3path_table[j][A][B][C].before_C = C;
						dp3path_table[j][A][B][C].m = 2;
					}
					else if (fc <= fa && fc <= fb)
					{
						dp3_table[j][A][B][C] = fc;
						dp3path_table[j][A][B][C].before_A = A;
						dp3path_table[j][A][B][C].before_B = B;
						dp3path_table[j][A][B][C].before_C = rc;
						dp3path_table[j][A][B][C].m = 3;
					}
				}
			}
		}
	}
	print_dp3pathtable();
	dp3result = dp3_table[current_config.n][current_config.D][current_config.D][current_config.D];
	QueryPerformanceCounter(&t2);
	dp3_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_dptable3();
	return dp3result;
}