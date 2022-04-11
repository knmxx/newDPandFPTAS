#pragma once

#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "job.h"
#include "tool.h"
using namespace std;

//MISHA
//去掉循环并且输出调度方案
//去掉dispath方法直接写

struct ConDP43
{
	int x;//收益
	string m1 = "";//存储第一台机器加工的任务
	string m2 = "";//存储第二台机器加工的任务
	string m3 = "";//存储第三台机器加工的任务
} condp43;

int dp43result = -1;
double dp43_running_time = -1;
Job dp43job_array[MAX_N];
ConDP43*** dp43table;//解集  长度：  d+Pmax+1*2
int** Q4;//队列

int pmax43 = 0;


void new_dp43table()
{
	dp43table = new ConDP43**[current_config.D + pmax43 + 1];
	Q4 = new int*[(current_config.D + pmax43 + 1)*
		(current_config.D + pmax43 + 1)*
		(current_config.D + pmax43 + 1)];
	for (int i = 0; i < (current_config.D + pmax43 + 1); i++)
	{
		dp43table[i] = new ConDP43*[current_config.D + pmax43 + 1];
	}
	for (int i = 0; i < (current_config.D + pmax43 + 1); i++)
		for (int j = 0; j < (current_config.D + pmax43 + 1); j++)
		{
			dp43table[i][j] = new ConDP43[current_config.D + pmax43 + 1];
		}
	for (int i = 0; i < (current_config.D + pmax43 + 1)*(current_config.D + pmax43 + 1)*(current_config.D + pmax43 + 1); i++)
	{
		Q4[i] = new int[3];
	}
	for (int i = 0; i < (current_config.D + pmax43 + 1); i++)
		for (int j = 0; j < (current_config.D + pmax43 + 1); j++)
			for (int k = 0; k < (current_config.D + pmax43 + 1); k++)
			{
				dp43table[i][j][k].x = -1;
			}
}

void free_dptable43()
{
	for (int i = 0; i < (current_config.D + pmax43 + 1); i++)
	{
		for (int j = 0; j < (current_config.D + pmax43 + 1); j++)
		{
			delete[] dp43table[i][j];
		}
		delete[] dp43table[i];
	}
	for (int i = 0; i < (current_config.D + pmax43 + 1)*(current_config.D + pmax43 + 1)*(current_config.D + pmax43 + 1); i++)
	{
		delete[] Q4[i];
	}
	delete[] dp43table;
	delete[] Q4;
}



int dp43()
{
	memcpy(dp43job_array, job_set, sizeof(job_set));//任务集
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	dp43result = -1;

	//根据LPT规则从大到小排序加工时间
	//sort(dp43job_array, dp43job_array + current_config.n, cmp);
	pmax43 = 0;
	for (int i = 0; i < current_config.n; i++)
	{
		if (pmax43 < dp43job_array[i].proc_time)
			pmax43 = dp43job_array[i].proc_time;
	}
	//创建数组并且初始化
	new_dp43table();

	int m1 = 0, m2 = 0, m3 = 0, m4 = 0, x = 0;//四台机器
	//初始化第一个数
	m1 = 0;
	m2 = 0;
	m3 = 0;
	m4 = PSUM - m1 - m2 - m3;
	x = ((m1 > current_config.D) ? current_config.D : m1)
		+ ((m2 > current_config.D) ? current_config.D : m2)
		+ ((m3 > current_config.D) ? current_config.D : m3)
		+ ((m4 > current_config.D) ? current_config.D : m4);
	dp43table[0][0][0].x = x;
	dp43table[0][0][0].m1 = "";
	dp43table[0][0][0].m2 = "";
	dp43table[0][0][0].m3 = "";
	Q4[0][0] = m1;
	Q4[0][1] = m2;
	Q4[0][2] = m3;
	int job_num = 1;//第i个任务
	int q_length = 1;//存储上一次迭代时q里的元素个数
	//开始迭代
	while (job_num <= current_config.n)
	{
		int processtime = dp43job_array[job_num - 1].proc_time;//获取第i个任务的处理时间
		int num = 0;//计数当前任务加了多少次
		//循环Q里的每个元素和第i个任务相加


		for (int i = 0; i < q_length; i++)
		{
			int a = 0, b = 0, c = 0;
			//当第i个任务在第一个机器上加工
			a = processtime + Q4[i][0];
			b = Q4[i][1];
			c = Q4[i][2];
			if (a < (current_config.D + pmax43) && b < (current_config.D + pmax43) && c < (current_config.D + pmax43))
			{
				m1 = a;
				m2 = b;
				m3 = c;
				m4 = PSUM - a - b - c;
				x = ((m1 > current_config.D) ? current_config.D : m1)
					+ ((m2 > current_config.D) ? current_config.D : m2)
					+ ((m3 > current_config.D) ? current_config.D : m3)
					+ ((m4 > current_config.D) ? current_config.D : m4);
				if (dp43table[a][b][c].x == -1)
				{
					dp43table[a][b][c].x = x;
					//把加之前的数组都复制过来  包括m1和m2的
					dp43table[a][b][c].m1 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m1 + 
						to_string(job_num) + " ";
					dp43table[a][b][c].m2 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m2;
					dp43table[a][b][c].m3 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m3;
					//把当前任务加到m1上
					//dp43table[a][b][c].m1 += to_string(job_num) + " ";//当前处理的任务  任务编号从1开始
					Q4[q_length + num][0] = a;//q_length从0开始
					Q4[q_length + num][1] = b;//q_length从0开始
					Q4[q_length + num][2] = c;//q_length从0开始
					num++;
				}
			}
			//当第i个任务在第二个机器上加工
			a = Q4[i][0];
			b = processtime + Q4[i][1];
			c = Q4[i][2];
			if (a < (current_config.D + pmax43) && b < (current_config.D + pmax43) && c < (current_config.D + pmax43))
			{
				m1 = a;
				m2 = b;
				m3 = c;
				m4 = PSUM - a - b - c;
				x = ((m1 > current_config.D) ? current_config.D : m1)
					+ ((m2 > current_config.D) ? current_config.D : m2)
					+ ((m3 > current_config.D) ? current_config.D : m3)
					+ ((m4 > current_config.D) ? current_config.D : m4);
				if (dp43table[a][b][c].x == -1)
				{
					dp43table[a][b][c].x = x;
					//把加之前的数组都复制过来  包括m1和m2的
					dp43table[a][b][c].m1 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m1;
					dp43table[a][b][c].m2 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m2 + to_string(job_num) + " ";
					dp43table[a][b][c].m3 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m3;
					//把当前任务加到m2上
					//dp43table[a][b][c].m2 += to_string(job_num) + " ";//当前处理的任务  任务编号从1开始
					Q4[q_length + num][0] = a;//q_length从0开始
					Q4[q_length + num][1] = b;//q_length从0开始
					Q4[q_length + num][2] = c;//q_length从0开始
					num++;
				}
			}
			//当第i个任务在第二个机器上加工
			a = Q4[i][0];
			b = Q4[i][1];
			c = processtime + Q4[i][2];
			if (a < (current_config.D + pmax43) && b < (current_config.D + pmax43) && c < (current_config.D + pmax43))
			{
				m1 = a;
				m2 = b;
				m3 = c;
				m4 = PSUM - a - b - c;
				x = ((m1 > current_config.D) ? current_config.D : m1)
					+ ((m2 > current_config.D) ? current_config.D : m2)
					+ ((m3 > current_config.D) ? current_config.D : m3)
					+ ((m4 > current_config.D) ? current_config.D : m4);
				if (dp43table[a][b][c].x == -1)
				{
					dp43table[a][b][c].x = x;
					//把加之前的数组都复制过来  包括m1和m2的
					dp43table[a][b][c].m1 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m1;
					dp43table[a][b][c].m2 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m2;
					dp43table[a][b][c].m3 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m3 + to_string(job_num) + " ";
					//把当前任务加到m3上
					//dp43table[a][b][c].m3 += to_string(job_num) + " ";//当前处理的任务  任务编号从1开始
					Q4[q_length + num][0] = a;//q_length从0开始
					Q4[q_length + num][1] = b;//q_length从0开始
					Q4[q_length + num][2] = c;//q_length从0开始
					num++;
				}
			}
		}

		/*for (int i = 0; i < 100; i++)
			cout << Q3[i][0] << "  ";
		cout << endl;*/
		q_length += num;
		//cout << "length------------" << q_length << endl;
		job_num++;
	}
	int bestresult_1 = 0, bestresult_2 = 0, bestresult_3 = 0;//最好的结果的下标  三维数组  三下标
	for (int i = 0; i < current_config.D + pmax43 + 1; i++)
	{
		for (int j = 0; j < current_config.D + pmax43 + 1; j++)
		{
			for (int k = 0; k < current_config.D + pmax43 + 1; k++)
			{
				if (dp43table[i][j][k].x > dp43result)
				{
					dp43result = dp43table[i][j][k].x;
					bestresult_1 = i;
					bestresult_2 = j;
					bestresult_3 = k;
				}
			}
		}
	}

	m1 = 0, m2 = 0, m3 = 0, m4 = 0, x = 0;
	//获得了最好的结果的下标  把任务提取出来算一下加工收益
	//cout << endl << "交付期：" << current_config.D << endl;
	//cout << fptas33table[bestresult_1][bestresult_2].m1  ;
	//cout << dp43table[bestresult_1][bestresult_2][bestresult_3].m1 << endl;
	//cout << dp43table[bestresult_1][bestresult_2][bestresult_3].m2 << endl;
	//cout << dp43table[bestresult_1][bestresult_2][bestresult_3].m3 << endl;
	m1 = sqlit_sum(dp43table[bestresult_1][bestresult_2][bestresult_3].m1);
	m2 = sqlit_sum(dp43table[bestresult_1][bestresult_2][bestresult_3].m2);
	m3 = sqlit_sum(dp43table[bestresult_1][bestresult_2][bestresult_3].m3);
	m4 = PSUM - m1 - m2 - m3;
	x = ((m1 > current_config.D) ? current_config.D : m1) +
		((m2 > current_config.D) ? current_config.D : m2) +
		((m3 > current_config.D) ? current_config.D : m3) +
		((m4 > current_config.D) ? current_config.D : m4);

	QueryPerformanceCounter(&t2);
	dp43_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_dptable43();

	return x;
}
