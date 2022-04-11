#pragma once
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <math.h>
#include "job.h"
using namespace std;
struct Path21
{
public:
	int before_A;
	int before_B;
	int m;
};


double dpe21_running_time = -1;
double *** dpe21_table;
Path21 *** path21_table;
int comple21_table[2];
const double e21 = 0.05;
double dpe21_d = 0.0;
int table21_length = 0;
double K21 = 0.0;
double dpe21_result = 0.0;
Job job21_array[MAX_N];
int real21_result = 0;
void new_dpetable21()
{
	dpe21_table = new double**[current_config.n + 1];
	path21_table = new Path21 **[current_config.n + 1];
	for (int i = 0; i < current_config.n + 1; ++i)
	{
		dpe21_table[i] = new double*[table21_length + 1];
		path21_table[i] = new Path21 *[table21_length + 1];
	}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < table21_length + 1; j++)
		{
			dpe21_table[i][j] = new double[table21_length + 1];
			path21_table[i][j] = new Path21[table21_length + 1];
		}
}
void init_dpetable21()
{
	memset(comple21_table, 0, sizeof(comple21_table));
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= table21_length; ++j)
		{
			for (int r = 0; r <= table21_length; ++r)
			{
				dpe21_table[i][j][r] = 0.0;
				path21_table[i][j][r].before_A = 0;
				path21_table[i][j][r].before_B = 0;
				path21_table[i][j][r].m = 0;
			}
		}

	}
}
void printf_dpetable21()
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
void get_real_result21()
{
	real21_result = 0;
	Path21 current_job = path21_table[current_config.n][table21_length][table21_length];
	for (int i = current_config.n; i > 0; --i)
	{

		if (current_job.m == 1)
		{
			comple21_table[0] += job21_array[i - 1].proc_time;
			//cout << job21_array[i - 1].proc_time << endl;
			real21_result += job_late_work(job21_array[i - 1], comple21_table[0]);
		}
		if (current_job.m == 2)
		{
			comple21_table[1] += job21_array[i - 1].proc_time;
			real21_result += job_late_work(job21_array[i - 1], comple21_table[1]);
		}
		current_job = path21_table[i - 1][current_job.before_A][current_job.before_B];
	}
}
void free_dpetable21()
{
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= table21_length; ++j)
		{
			delete[] dpe21_table[i][j];
			delete[] path21_table[i][j];
		}
		delete[] dpe21_table[i];
		delete[] path21_table[i];
	}
	delete[] dpe21_table;
	delete[] path21_table;
}
int dpe21()
{
	memcpy(job21_array, job_set, sizeof(job_set));
	K21 = 1.0 * e21 * current_config.D / (current_config.m + current_config.n);
	dpe21_d = 1.0 * K21 * floor((1.0 * current_config.D / K21) + 1);
	for (int i = 0; i < current_config.n; ++i)
	{
		job21_array[i].newp = K21 * floor(1.0 * job21_array[i].proc_time / K21);
		job21_array[i].newd = K21 * floor((1.0 * job21_array[i].due_date / K21) + 1);
	}

	//table21_length = (int)(1.0 * dpe21_d / K21);
	table21_length = (int)(1.0 * dpe21_d/ K21);
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);
	new_dpetable21();
	init_dpetable21();
	for (int j = 1; j <= current_config.n; ++j)
	{
		for (int A = 0; A <= table21_length; ++A)
		{
			for (int B = 0; B <= table21_length; ++B)
			{
				double ya = max(job21_array[j - 1].newp - K21 * A, 0.0);
				double yb = max(job21_array[j - 1].newp - K21 * B, 0.0);
				int ra = max((int)((K21 * A - job21_array[j - 1].newp) / K21), 0);
				int rb = max((int)((K21 * B - job21_array[j - 1].newp) / K21), 0);
				double fa = dpe21_table[j - 1][ra][B] + ya;
				double fb = dpe21_table[j - 1][A][rb] + yb;
				if (fa < fb || fabs(fa - fb) < eps)
				{
					dpe21_table[j][A][B] = fa;
					path21_table[j][A][B].before_A = ra;
					path21_table[j][A][B].before_B = B;
					path21_table[j][A][B].m = 1;
				}
				else
				{
					dpe21_table[j][A][B] = fb;
					path21_table[j][A][B].before_A = A;
					path21_table[j][A][B].before_B = rb;
					path21_table[j][A][B].m = 2;
				}

			}
		}
	}
	dpe21_result = dpe21_table[current_config.n][table21_length][table21_length];
	get_real_result21();
	QueryPerformanceCounter(&t2);
	dpe21_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_dpetable21();
	return real21_result;
}