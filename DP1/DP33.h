#pragma once

#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "job.h"
#include "tool.h"
using namespace std;
//去掉循环并且输出调度方案

//三台机器
struct ConDP33
{
	int x;//收益
	string m1 = "";//存储第一台机器加工的任务
	string m2 = "";//存储第二台机器加工的任务
} condp33;

int dp33result = -1;
double dp33_running_time = -1;
Job dp33job_array[MAX_N];
ConDP33** dp33table;//解集  长度：  d+Pmax+1*2
int** Q3;//队列

int pmax33 = 0;

void new_dp33table()
{
	dp33table = new ConDP33*[current_config.D + pmax33 + 1];
	Q3 = new int*[(current_config.D + pmax33 + 1)*(current_config.D + pmax33 + 1)];
	for (int i = 0; i < (current_config.D + pmax33 + 1); i++)
	{
		dp33table[i] = new ConDP33[current_config.D + pmax33 + 1];
	}
	for (int i = 0; i < (current_config.D + pmax33 + 1)*(current_config.D + pmax33 + 1); i++)
	{
		Q3[i] = new int[2];
	}
	for (int i = 0; i < (current_config.D + pmax33 + 1); i++)
		for (int j = 0; j < (current_config.D + pmax33 + 1); j++)
		{
			dp33table[i][j].x = -1;
		}
}

void free_dptable33()
{
	for (int i = 0; i < (current_config.D + pmax33 + 1); i++)
	{
		delete[] dp33table[i];
	}
	for (int i = 0; i < (current_config.D + pmax33 + 1)*(current_config.D + pmax33 + 1); i++)
	{
		delete[] Q3[i];
	}
	delete[] dp33table;
	delete[] Q3;
}

int dp33()
{
	memcpy(dp33job_array, job_set, sizeof(job_set));//任务集
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	dp33result = -1;

	//根据LPT规则从大到小排序加工时间
	//sort(dp33job_array, dp33job_array + current_config.n, cmp);
	pmax33 = 0;
	for (int i = 0; i < current_config.n; i++)
	{
		if (pmax33 < dp33job_array[i].proc_time)
			pmax33 = dp33job_array[i].proc_time;
	}
	//创建数组并且初始化
	new_dp33table();

	int m1 = 0, m2 = 0, m3 = 0, x = 0;//两台机器
	//初始化第一个数
	m1 = 0;
	m2 = 0;
	m3 = PSUM - m1 - m2;
	x = ((m1 > current_config.D) ? current_config.D : m1)
		+ ((m2 > current_config.D) ? current_config.D : m2)
		+ ((m3 > current_config.D) ? current_config.D : m3);
	dp33table[0][0].x = x;
	dp33table[0][0].m1 = "";
	dp33table[0][0].m2 = "";
	Q3[0][0] = m1;
	Q3[0][1] = m2;
	int job_num = 1;//第i个任务
	int q_length = 1;//存储上一次迭代时q里的元素个数
	//开始迭代
	while (job_num <= current_config.n)
	{
		int processtime = dp33job_array[job_num - 1].proc_time;//获取第i个任务的处理时间
		int num = 0;//计数当前任务加了多少次
		//循环Q里的每个元素和第i个任务相加


		for (int i = 0; i < q_length; i++)
		{
			int a = 0, b = 0;
			//当第i个任务在第一个机器上加工
			a = processtime + Q3[i][0];
			b = Q3[i][1];
			if (a < (current_config.D + pmax33) && b < (current_config.D + pmax33))
			{
				m1 = a;
				m2 = b;
				m3 = PSUM - a - b;
				x = ((m1 > current_config.D) ? current_config.D : m1)
					+ ((m2 > current_config.D) ? current_config.D : m2)
					+ ((m3 > current_config.D) ? current_config.D : m3);
				if (dp33table[a][b].x == -1)
				{
					dp33table[a][b].x = x;
					//把加之前的数组都复制过来  包括m1和m2的
					dp33table[a][b].m1 = dp33table[Q3[i][0]][Q3[i][1]].m1;
					dp33table[a][b].m2 = dp33table[Q3[i][0]][Q3[i][1]].m2;
					//把当前任务加到m1上
					dp33table[a][b].m1 += to_string(job_num) + " ";//当前处理的任务  任务编号从1开始
					Q3[q_length + num][0] = a;//q_length从0开始
					Q3[q_length + num][1] = b;//q_length从0开始
					num++;
				}
			}
			//当第i个任务在第二个机器上加工
			a = Q3[i][0];
			b = processtime + Q3[i][1];
			if (a < (current_config.D + pmax33) && b < (current_config.D + pmax33))
			{
				m1 = a;
				m2 = b;
				m3 = PSUM - a - b;
				x = ((m1 > current_config.D) ? current_config.D : m1)
					+ ((m2 > current_config.D) ? current_config.D : m2)
					+ ((m3 > current_config.D) ? current_config.D : m3);
				if (dp33table[a][b].x == -1)
				{
					dp33table[a][b].x = x;
					//把加之前的数组都复制过来  包括m1和m2的
					dp33table[a][b].m1 = dp33table[Q3[i][0]][Q3[i][1]].m1;
					dp33table[a][b].m2 = dp33table[Q3[i][0]][Q3[i][1]].m2;
					//把当前任务加到m2上
					dp33table[a][b].m2 += to_string(job_num) + " ";//当前处理的任务  任务编号从1开始
					Q3[q_length + num][0] = a;//q_length从0开始
					Q3[q_length + num][1] = b;//q_length从0开始
					num++;
				}

				//想再改进一下   改进很成功  可行解减少一半  但是时间没什么太大差别
				//if (dp33table[a][b] == -1&& dp33table[b][a] == -1)
				//{
				//	dp33table[a][b] = x;
				//	Q3[q_length + num][0] = a;//q_length从0开始
				//	Q3[q_length + num][1] = b;//q_length从0开始
				//	num++;
				//}
			}
		}

		/*for (int i = 0; i < 100; i++)
			cout << Q3[i][0] << "  ";
		cout << endl;*/
		q_length += num;
		//cout << "length------------" << q_length << endl;
		job_num++;
	}
	int bestresult_1 = 0, bestresult_2 = 0;//最好的结果的下标  二维数组  俩下标
	for (int i = 0; i < current_config.D + pmax33 + 1; i++)
	{
		for (int j = 0; j < current_config.D + pmax33 + 1; j++)
		{
			if (dp33table[i][j].x > dp33result)
			{
				dp33result = dp33table[i][j].x;
				bestresult_1 = i;
				bestresult_2 = j;
			}
		}
	}
	m1 = 0, m2 = 0, m3 = 0, x = 0;
	//获得了最好的结果的下标  把任务提取出来算一下加工收益
	//cout << endl << "交付期：" << current_config.D << endl;
	//cout << fptas33table[bestresult_1][bestresult_2].m1  ;
	m1 = sqlit_sum(dp33table[bestresult_1][bestresult_2].m1);
	m2 = sqlit_sum(dp33table[bestresult_1][bestresult_2].m2);
	m3 = PSUM - m1 - m2;
	x = ((m1 > current_config.D) ? current_config.D : m1) +
		((m2 > current_config.D) ? current_config.D : m2) +
		((m3 > current_config.D) ? current_config.D : m3);

	QueryPerformanceCounter(&t2);
	dp33_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_dptable33();

	return x;
}
