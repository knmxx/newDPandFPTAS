#pragma once
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <math.h>
#include "job.h"
using namespace std;
struct Path31
{
public:
	int before_A;
	int before_B;
	int before_C;
	int m;
};


double dpe31_running_time = -1;
double **** dpe31_table;
Path31 **** path31_table;
int comple31_table[3];
const double e31 = 0.05;
double dpe31_d = 0.0;
int table31_length = 0;
double K31 = 0.0;
double dpe31_result = 0.0;
Job job31_array[MAX_N];
int real31_result = 0;
void new_dpetable31()
{
	dpe31_table = new double***[current_config.n + 1];
	path31_table = new Path31 ***[current_config.n + 1];
	for (int i = 0; i < current_config.n + 1; ++i)
	{
		dpe31_table[i] = new double**[table31_length + 1];
		path31_table[i] = new Path31 **[table31_length + 1];
	}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < table31_length + 1; j++)
		{
			dpe31_table[i][j] = new double*[table31_length + 1];
			path31_table[i][j] = new Path31*[table31_length + 1];
		}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < table31_length + 1; j++)
			for (int k = 0; k < table31_length + 1; k++)
			{
				dpe31_table[i][j][k] = new double[table31_length + 1];
				path31_table[i][j][k] = new Path31[table31_length + 1];
			}
}
void init_dpetable31()
{
	memset(comple31_table, 0, sizeof(comple31_table));
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= table31_length; ++j)
		{
			for (int r = 0; r <= table31_length; ++r)
			{
				for (int x = 0; x <= table31_length; ++x)
				{
					dpe31_table[i][j][r][x] = 0.0;
					path31_table[i][j][r][x].before_A = 0;
					path31_table[i][j][r][x].before_B = 0;
					path31_table[i][j][r][x].before_C = 0;
					path31_table[i][j][r][x].m = 0;
				}
			}
		}

	}
}
void printf_dpetable31()
{
	/*for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= table21_length; ++j)
		{
			for (int r = 0; r <= table21_length; ++r)
			{
				printf("f(%d,%.1f,%.1f)=%.1f ", i, K21 * j, K21 * r, dpe21_table[i][j][r]);
			}
			printf("\n");
		}
		printf("\n");
	}*/

}
void get_real_result31()
{
	real31_result = 0;
	Path31 current_job = path31_table[current_config.n][table31_length][table31_length][table31_length];
	for (int i = current_config.n; i > 0; --i)
	{

		if (current_job.m == 1)
		{
			comple31_table[0] += job31_array[i - 1].proc_time;
			real31_result += job_late_work(job31_array[i - 1], comple31_table[0]);
		}
		if (current_job.m == 2)
		{
			comple31_table[1] += job31_array[i - 1].proc_time;
			real31_result += job_late_work(job31_array[i - 1], comple31_table[1]);
		}
		if (current_job.m == 3)
		{
			comple31_table[2] += job31_array[i - 1].proc_time;
			real31_result += job_late_work(job31_array[i - 1], comple31_table[2]);
		}
		current_job = path31_table[i - 1][current_job.before_A][current_job.before_B][current_job.before_C];
	}
}
void free_dpetable31()
{
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= table31_length; ++j)
		{
			for (int k = 0; k <= table31_length; ++k)
			{
				delete[] dpe31_table[i][j][k];
				delete[] path31_table[i][j][k];
			}
			delete[] dpe31_table[i][j];
			delete[] path31_table[i][j];
		}
		delete[] dpe31_table[i];
		delete[] path31_table[i];
	}
	delete[] dpe31_table;
	delete[] path31_table;
}
int dpe31()
{
	memcpy(job31_array, job_set, sizeof(job_set));
	K31 = 1.0 * e31 * current_config.D / (current_config.m + current_config.n);
	dpe31_d = 1.0 * K31 * floor((1.0 * current_config.D / K31) + 1);
	for (int i = 0; i < current_config.n; ++i)
	{
		job31_array[i].newp = K31 * floor(1.0 * job31_array[i].proc_time / K31);
		cout << job31_array[i].newp<<endl;
		job31_array[i].newd = K31 * floor((1.0 * job31_array[i].due_date / K31) + 1);
	}

	//table21_length = (int)(1.0 * dpe21_d / K21);
	table31_length = (int)(1.0 * dpe31_d / K31);
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);
	new_dpetable31();
	init_dpetable31();
	for (int j = 1; j <= current_config.n; ++j)
	{
		for (int A = 0; A <= table31_length; ++A)
		{
			for (int B = 0; B <= table31_length; ++B)
			{
				for (int C = 0; C <= table31_length; ++C)
				{
					double ya = max(job31_array[j - 1].newp - K31 * A, 0.0);
					double yb = max(job31_array[j - 1].newp - K31 * B, 0.0);
					double yc = max(job31_array[j - 1].newp - K31 * C, 0.0);
					int ra = max((int)((K31 * A - job31_array[j - 1].newp) / K31), 0);
					int rb = max((int)((K31 * B - job31_array[j - 1].newp) / K31), 0);
					int rc = max((int)((K31 * C - job31_array[j - 1].newp) / K31), 0);
					double fa = dpe31_table[j - 1][ra][B][C] + ya;
					double fb = dpe31_table[j - 1][A][rb][C] + yb;
					double fc = dpe31_table[j - 1][A][B][rc] + yc;
					if ((fa < fb || fabs(fa - fb) < eps) && (fa < fc || fabs(fa - fc) < eps)) 
					{
						dpe31_table[j][A][B][C] = fa;
						path31_table[j][A][B][C].before_A = ra;
						path31_table[j][A][B][C].before_B = B;
						path31_table[j][A][B][C].before_C = C;
						path31_table[j][A][B][C].m = 1;
					}
					else if ((fb < fa || fabs(fb - fa) < eps) && (fb < fc || fabs(fb - fc) < eps))
					{
						dpe31_table[j][A][B][C] = fb;
						path31_table[j][A][B][C].before_A = A;
						path31_table[j][A][B][C].before_B = rb;
						path31_table[j][A][B][C].before_C = C;
						path31_table[j][A][B][C].m = 2;
					}
					else if ((fc < fa || fabs(fc - fa) < eps) && (fc < fb || fabs(fc - fb) < eps))
					{
						dpe31_table[j][A][B][C] = fc;
						path31_table[j][A][B][C].before_A = A;
						path31_table[j][A][B][C].before_B = B;
						path31_table[j][A][B][C].before_C = rc;
						path31_table[j][A][B][C].m = 3;
					}
				}
			}
		}
	}
	dpe31_result = dpe31_table[current_config.n][table31_length][table31_length][table31_length];
	get_real_result31();
	QueryPerformanceCounter(&t2);
	dpe31_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_dpetable31();
	return real31_result;
}
