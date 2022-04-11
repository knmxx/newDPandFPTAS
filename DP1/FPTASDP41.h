#pragma once
#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <math.h>
#include "job.h"
using namespace std;
struct Path41
{
public:
	int before_A;
	int before_B;
	int before_C;
	int before_D;
	int m;
};


double dpe41_running_time = -1;
double ***** dpe41_table;
Path41 ***** path41_table;
int comple41_table[4];
const double e41 = 0.05;
double dpe41_d = 0.0;
int table41_length = 0;
double K41 = 0.0;
double dpe41_result = 0.0;
Job job41_array[MAX_N];
int real41_result = 0;
void new_dpetable41()
{
	dpe41_table = new double****[current_config.n + 1];
	path41_table = new Path41 ****[current_config.n + 1];
	for (int i = 0; i < current_config.n + 1; ++i)
	{
		dpe41_table[i] = new double***[table41_length + 1];
		path41_table[i] = new Path41 ***[table41_length + 1];
	}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < table41_length + 1; j++)
		{
			dpe41_table[i][j] = new double**[table41_length + 1];
			path41_table[i][j] = new Path41**[table41_length + 1];
		}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < table41_length + 1; j++)
			for (int k = 0; k < table41_length + 1; k++)
			{
				dpe41_table[i][j][k] = new double*[table41_length + 1];
				path41_table[i][j][k] = new Path41*[table41_length + 1];
			}
	for (int i = 0; i < current_config.n + 1; i++)
		for (int j = 0; j < table41_length + 1; j++)
			for (int k = 0; k < table41_length + 1; k++)
				for (int x = 0; x < table41_length + 1; x++)
				{
					dpe41_table[i][j][k][x] = new double[table41_length + 1];
					path41_table[i][j][k][x] = new Path41[table41_length + 1];
				}
}
void init_dpetable41()
{
	memset(comple41_table, 0, sizeof(comple41_table));
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= table41_length; ++j)
		{
			for (int r = 0; r <= table41_length; ++r)
			{
				for (int x = 0; x <= table41_length; ++x)
				{
					for (int y = 0; y <= table41_length; ++y)
					{
						dpe41_table[i][j][r][x][y] = 0.0;
						path41_table[i][j][r][x][y].before_A = 0;
						path41_table[i][j][r][x][y].before_B = 0;
						path41_table[i][j][r][x][y].before_C = 0;
						path41_table[i][j][r][x][y].before_D = 0;
						path41_table[i][j][r][x][y].m = 0;
					}
				}
			}
		}

	}
}
void printf_dpetable41()
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
void get_real_result41()
{
	real41_result = 0;
	Path41 current_job = path41_table[current_config.n][table41_length][table41_length][table41_length][table41_length];
	for (int i = current_config.n; i > 0; --i)
	{

		if (current_job.m == 1)
		{
			comple41_table[0] += job41_array[i - 1].proc_time;
			real41_result += job_late_work(job41_array[i - 1], comple41_table[0]);
		}
		if (current_job.m == 2)
		{
			comple41_table[1] += job41_array[i - 1].proc_time;
			real41_result += job_late_work(job41_array[i - 1], comple41_table[1]);
		}
		if (current_job.m == 3)
		{
			comple41_table[2] += job41_array[i - 1].proc_time;
			real41_result += job_late_work(job41_array[i - 1], comple41_table[2]);
		}
		if (current_job.m == 4)
		{
			comple41_table[3] += job41_array[i - 1].proc_time;
			real41_result += job_late_work(job41_array[i - 1], comple41_table[3]);
		}
		current_job = path41_table[i - 1][current_job.before_A][current_job.before_B][current_job.before_C][current_job.before_D];
	}
}
void free_dpetable41()
{
	for (int i = 0; i <= current_config.n; ++i)
	{
		for (int j = 0; j <= table41_length; ++j)
		{
			for (int k = 0; k <= table41_length; ++k)
			{
				for (int x = 0; x <= table41_length; ++x)
				{
					delete[] dpe41_table[i][j][k][x];
					delete[] path41_table[i][j][k][x];
				}
				delete[] dpe41_table[i][j][k];
				delete[] path41_table[i][j][k];
			}
			delete[] dpe41_table[i][j];
			delete[] path41_table[i][j];
		}
		delete[] dpe41_table[i];
		delete[] path41_table[i];
	}
	delete[] dpe41_table;
	delete[] path41_table;
}
int dpe41()
{
	memcpy(job41_array, job_set, sizeof(job_set));
	K41 = 1.0 * e41 * current_config.D / (current_config.m + current_config.n);
	//cout << endl << K41 << endl;
	dpe41_d = 1.0 * K41 * floor((1.0 * current_config.D / K41) + 1);
	for (int i = 0; i < current_config.n; ++i)
	{
		job41_array[i].newp = K41 * floor(1.0 * job41_array[i].proc_time / K41);
		//cout << endl << job41_array[i].newp << "   ";
		job41_array[i].newd = K41 * floor((1.0 * job41_array[i].due_date / K41) + 1);
	}

	//table21_length = (int)(1.0 * dpe21_d / K21);
	table41_length = (int)(1.0 * dpe41_d / K41);
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);
	new_dpetable41();
	init_dpetable41();
	for (int j = 1; j <= current_config.n; ++j)
	{
		for (int A = 0; A <= table41_length; ++A)
		{
			for (int B = 0; B <= table41_length; ++B)
			{
				for (int C = 0; C <= table41_length; ++C)
				{
					for (int D = 0; D <= current_config.D; ++D)
					{
						double ya = max(job41_array[j - 1].newp - K41 * A, 0.0);
						double yb = max(job41_array[j - 1].newp - K41 * B, 0.0);
						double yc = max(job41_array[j - 1].newp - K41 * C, 0.0);
						double yd = max(job41_array[j - 1].newp - K41 * D, 0.0);
						int ra = max((int)((K41 * A - job41_array[j - 1].newp) / K41), 0);
						int rb = max((int)((K41 * B - job41_array[j - 1].newp) / K41), 0);
						int rc = max((int)((K41 * C - job41_array[j - 1].newp) / K41), 0);
						int rd = max((int)((K41 * D - job41_array[j - 1].newp) / K41), 0);
						double fa = dpe41_table[j - 1][ra][B][C][D] + ya;
						double fb = dpe41_table[j - 1][A][rb][C][D] + yb;
						double fc = dpe41_table[j - 1][A][B][rc][D] + yc;
						double fd = dpe41_table[j - 1][A][B][C][rd] + yd;
						if ((fa < fb || fabs(fa - fb) < eps) && (fa < fc || fabs(fa - fc) < eps)
							&& (fa < fd || fabs(fa - fd) < eps))
						{
							dpe41_table[j][A][B][C][D] = fa;
							path41_table[j][A][B][C][D].before_A = ra;
							path41_table[j][A][B][C][D].before_B = B;
							path41_table[j][A][B][C][D].before_C = C;
							path41_table[j][A][B][C][D].before_D = D;
							path41_table[j][A][B][C][D].m = 1;
						}
						else if ((fb < fa || fabs(fb - fa) < eps) && (fb < fc || fabs(fb - fc) < eps)
							&& (fb < fd || fabs(fb - fd) < eps))
						{
							dpe41_table[j][A][B][C][D] = fb;
							path41_table[j][A][B][C][D].before_A = A;
							path41_table[j][A][B][C][D].before_B = rb;
							path41_table[j][A][B][C][D].before_C = C;
							path41_table[j][A][B][C][D].before_D = D;
							path41_table[j][A][B][C][D].m = 2;
						}
						else if ((fc < fa || fabs(fc - fa) < eps) && (fc < fb || fabs(fc - fb) < eps)
							&& (fc < fd || fabs(fc - fd) < eps))
						{
							dpe41_table[j][A][B][C][D] = fc;
							path41_table[j][A][B][C][D].before_A = A;
							path41_table[j][A][B][C][D].before_B = B;
							path41_table[j][A][B][C][D].before_C = rc;
							path41_table[j][A][B][C][D].before_D = D;
							path41_table[j][A][B][C][D].m = 3;
						}
						else if ((fd < fa || fabs(fd - fa) < eps) && (fd < fb || fabs(fd - fb) < eps)
							&& (fd < fc || fabs(fd - fc) < eps))
						{
							dpe41_table[j][A][B][C][D] = fd;
							path41_table[j][A][B][C][D].before_A = A;
							path41_table[j][A][B][C][D].before_B = B;
							path41_table[j][A][B][C][D].before_C = C;
							path41_table[j][A][B][C][D].before_D = rd;
							path41_table[j][A][B][C][D].m = 4;
						}
					}
				}
			}
		}
	}
	dpe41_result = dpe41_table[current_config.n][table41_length][table41_length][table41_length][table41_length];
	get_real_result41();
	QueryPerformanceCounter(&t2);
	dpe41_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_dpetable41();
	return real41_result;
}
