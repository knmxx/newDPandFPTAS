#pragma once
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "job.h"
using namespace std;
struct DP4Path
{
public:
	int before_A;
	int before_B;
	int before_C;
	int before_D;
	int m;
};
DP4Path ***** dp4path_table;
int dp4comple_table[4];
double dp4_running_time = -1;
int ***** dp4_table;
int dp4result = -1;
Job dp4job_array[MAX_N];
void new_dptable4()
{
	dp4_table = new int****[current_config.n + 1];
	dp4path_table = new DP4Path ****[current_config.n + 1];
	for (int i = 0; i < current_config.n + 1; ++i)
	{
		dp4_table[i] = new int***[current_config.D + 1];
		dp4path_table[i] = new DP4Path ***[current_config.D + 1];
	}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < current_config.D + 1; j++)
		{
			dp4_table[i][j] = new int**[current_config.D + 1];
			dp4path_table[i][j] = new DP4Path**[current_config.D + 1];
		}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < current_config.D + 1; j++)
			for (int k = 0; k < current_config.D + 1; k++)
			{
				dp4_table[i][j][k] = new int*[current_config.D + 1];
				dp4path_table[i][j][k] = new DP4Path*[current_config.D + 1];
			}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < current_config.D + 1; j++)
			for (int k = 0; k < current_config.D + 1; k++)
				for (int x = 0; x < current_config.D + 1; x++)
				{
					dp4_table[i][j][k][x] = new int[current_config.D + 1];
					dp4path_table[i][j][k][x] = new DP4Path[current_config.D + 1];
				}
}
void init_dptable4()
{
	memset(dp4comple_table, 0, sizeof(dp4comple_table));
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= current_config.D; ++j)
		{
			for (int k = 0; k <= current_config.D; ++k)
			{
				for (int x = 0; x <= current_config.D; ++x)
				{
					for (int y = 0; y <= current_config.D; ++y)
					{
						dp4_table[i][j][k][x][y] = 0;
						dp4path_table[i][j][k][x][y].before_A = 0;
						dp4path_table[i][j][k][x][y].before_B = 0;
						dp4path_table[i][j][k][x][y].before_C = 0;
						dp4path_table[i][j][k][x][y].before_D = 0;
						dp4path_table[i][j][k][x][y].m = 0;
					}
				}
			}
		}
	}
}
void print_dp4pathtable()
{
	dp4result = 0;
	DP4Path current_job = dp4path_table[current_config.n][current_config.D][current_config.D][current_config.D][current_config.D];
	for (int i = current_config.n; i > 0; --i)
	{

		if (current_job.m == 1)
		{
			dp4comple_table[0] += dp4job_array[i - 1].proc_time;
			dp4result += job_late_work(dp4job_array[i - 1], dp4comple_table[0]);
		}
		if (current_job.m == 2)
		{
			dp4comple_table[1] += dp4job_array[i - 1].proc_time;
			dp4result += job_late_work(dp4job_array[i - 1], dp4comple_table[1]);
		}
		if (current_job.m == 3)
		{
			dp4comple_table[2] += dp4job_array[i - 1].proc_time;
			dp4result += job_late_work(dp4job_array[i - 1], dp4comple_table[2]);
		}
		if (current_job.m == 4)
		{
			dp4comple_table[3] += dp4job_array[i - 1].proc_time;
			dp4result += job_late_work(dp4job_array[i - 1], dp4comple_table[3]);
		}
		current_job = dp4path_table[i - 1][current_job.before_A][current_job.before_B][current_job.before_C][current_job.before_D];
	}
}
void printf_dptable4()
{
	/*for (int i = 0; i <= current_config.n; ++i)
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
	}*/
}
void free_dptable4()
{
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= current_config.D; ++j)
		{
			for (int k = 0; k <= current_config.D; ++k)
			{
				for (int x = 0; x <= current_config.D; ++x)
				{
					delete[] dp4_table[i][j][k][x];
					delete[] dp4path_table[i][j][k][x];
				}
				delete[] dp4_table[i][j][k];
				delete[] dp4path_table[i][j][k];
			}
			delete[] dp4_table[i][j];
			delete[] dp4path_table[i][j];
		}
		delete[] dp4_table[i];
		delete[] dp4path_table[i];
	}
	delete[] dp4_table;
	delete[] dp4path_table;
}
int dp41()
{
	memcpy(dp4job_array, job_set, sizeof(job_set));
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);
	new_dptable4();
	init_dptable4();

	for (int j = 1; j <= current_config.n; ++j)
	{
		for (int A = 0; A <= current_config.D; ++A)
		{
			for (int B = 0; B <= current_config.D; ++B)
			{
				for (int C = 0; C <= current_config.D; ++C)
				{
					for (int D = 0; D <= current_config.D; ++D)
					{
						int ra = max(A - job_set[j - 1].proc_time, 0);
						int rb = max(B - job_set[j - 1].proc_time, 0);
						int rc = max(C - job_set[j - 1].proc_time, 0);
						int rd = max(D - job_set[j - 1].proc_time, 0);
						int ya = max(job_set[j - 1].proc_time - A, 0);
						int yb = max(job_set[j - 1].proc_time - B, 0);
						int yc = max(job_set[j - 1].proc_time - C, 0);
						int yd = max(job_set[j - 1].proc_time - D, 0);
						int fa = dp4_table[j - 1][ra][B][C][D] + ya;
						int fb = dp4_table[j - 1][A][rb][C][D] + yb;
						int fc = dp4_table[j - 1][A][B][rc][D] + yc;
						int fd = dp4_table[j - 1][A][B][C][rd] + yd;

						if (fa <= fb && fa <= fc && fa <= fd)
						{
							dp4_table[j][A][B][C][D] = fa;
							dp4path_table[j][A][B][C][D].before_A = ra;
							dp4path_table[j][A][B][C][D].before_B = B;
							dp4path_table[j][A][B][C][D].before_C = C;
							dp4path_table[j][A][B][C][D].before_D = D;
							dp4path_table[j][A][B][C][D].m = 1;
						}
						else if (fb <= fa && fb <= fc && fb <= fd)
						{
							dp4_table[j][A][B][C][D] = fb;
							dp4path_table[j][A][B][C][D].before_A = A;
							dp4path_table[j][A][B][C][D].before_B = rb;
							dp4path_table[j][A][B][C][D].before_C = C;
							dp4path_table[j][A][B][C][D].before_D = D;
							dp4path_table[j][A][B][C][D].m = 2;
						}
						else if (fc <= fa && fc <= fb && fc <= fd)
						{
							dp4_table[j][A][B][C][D] = fc;
							dp4path_table[j][A][B][C][D].before_A = A;
							dp4path_table[j][A][B][C][D].before_B = B;
							dp4path_table[j][A][B][C][D].before_C = rc;
							dp4path_table[j][A][B][C][D].before_D = D;
							dp4path_table[j][A][B][C][D].m = 3;
						}
						else if (fd <= fa && fd <= fb && fd <= fc)
						{
							dp4_table[j][A][B][C][D] = fd;
							dp4path_table[j][A][B][C][D].before_A = A;
							dp4path_table[j][A][B][C][D].before_B = B;
							dp4path_table[j][A][B][C][D].before_C = C;
							dp4path_table[j][A][B][C][D].before_D = rd;
							dp4path_table[j][A][B][C][D].m = 4;
						}
					}
				}
			}
		}
	}
	print_dp4pathtable();
	dp4result = dp4_table[current_config.n][current_config.D][current_config.D][current_config.D][current_config.D];
	QueryPerformanceCounter(&t2);
	dp4_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_dptable4();
	return dp4result;
}