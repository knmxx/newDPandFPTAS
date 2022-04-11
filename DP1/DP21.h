#pragma once
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "job.h"
using namespace std;
//YUAN
struct DP2Path
{
public:
	int before_A;
	int before_B;
	int m;
};
DP2Path *** dp2path_table;
int dp2comple_table[2];
double dp2_running_time = -1;
int *** dp2_table;
int dp2result = -1;
Job dp2job_array[MAX_N];
void new_dptable2()
{
	dp2_table = new int**[current_config.n + 1];
	dp2path_table = new DP2Path **[current_config.n + 1];
	for (int i = 0; i < current_config.n + 1; ++i)
	{
		dp2_table[i] = new int*[current_config.D + 1];
		dp2path_table[i] = new DP2Path *[current_config.D + 1];
	}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < current_config.D + 1; j++)
		{
			dp2_table[i][j] = new int[current_config.D + 1];
			dp2path_table[i][j] = new DP2Path[current_config.D + 1];
		}
}
void init_dptable2()
{
	memset(dp2comple_table, 0, sizeof(dp2comple_table));
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= current_config.D; ++j)
		{
			for (int k = 0; k <= current_config.D; ++k)
			{
				dp2_table[i][j][k] = 0;
				dp2path_table[i][j][k].before_A = 0;
				dp2path_table[i][j][k].before_B = 0;
				dp2path_table[i][j][k].m = 0;
			}
		}
	}
}
void print_dp2pathtable()
{
	dp2result = 0;
	DP2Path current_job = dp2path_table[current_config.n][current_config.D][current_config.D];
	for (int i = current_config.n; i > 0; --i)
	{

		if (current_job.m == 1)
		{
			dp2comple_table[0] += dp2job_array[i - 1].proc_time;
			dp2result += job_late_work(dp2job_array[i - 1], dp2comple_table[0]);
		}
		if (current_job.m == 2)
		{
			dp2comple_table[1] += dp2job_array[i - 1].proc_time;
			dp2result += job_late_work(dp2job_array[i - 1], dp2comple_table[1]);
		}
		current_job = dp2path_table[i - 1][current_job.before_A][current_job.before_B];
	}
}
void printf_dptable2()
{
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= current_config.D; ++j)
		{
			for (int k = 0; k <= current_config.D; ++k)
			{
				printf("f(%d,%d,%d)=%d ", i, j, k, dp2_table[i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
}
void free_dptable2()
{
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= current_config.D; ++j)
		{
			delete[] dp2_table[i][j];
			delete[] dp2path_table[i][j];
		}
		delete[] dp2_table[i];
		delete[] dp2path_table[i];
	}
	delete[] dp2_table;
	delete[] dp2path_table;
}
int dp21()
{
	memcpy(dp2job_array, job_set, sizeof(job_set));
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);
	new_dptable2();
	init_dptable2();

	for (int j = 1; j <= current_config.n; ++j)
	{
		for (int A = 0; A <= current_config.D; ++A)
		{
			for (int B = 0; B <= current_config.D; ++B)
			{
				int ra = max(A - job_set[j - 1].proc_time, 0);
				int rb = max(B - job_set[j - 1].proc_time, 0);
				int ya = max(job_set[j - 1].proc_time - A, 0);
				int yb = max(job_set[j - 1].proc_time - B, 0);
				int fa = dp2_table[j - 1][ra][B] + ya;
				int fb = dp2_table[j - 1][A][rb] + yb;
				if (fa <= fb)
				{
					dp2_table[j][A][B] = fa;
					dp2path_table[j][A][B].before_A = ra;
					dp2path_table[j][A][B].before_B = B;
					dp2path_table[j][A][B].m = 1;
				}
				else
				{
					dp2_table[j][A][B] = fb;
					dp2path_table[j][A][B].before_A = A;
					dp2path_table[j][A][B].before_B = rb;
					dp2path_table[j][A][B].m = 2;
				}
			}
		}
	}
	print_dp2pathtable();
	dp2result = dp2_table[current_config.n][current_config.D][current_config.D];
	QueryPerformanceCounter(&t2);
	dp2_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_dptable2();
	return dp2result;
}