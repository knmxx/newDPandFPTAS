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
struct ConDP23
{
	int x;//收益
	string m1 = "";//M1上的加工负载
} condp23;


int dp23result = -1;
double dp23_running_time = -1;
Job dp23job_array[MAX_N];
ConDP23* dp23table;//解集  长度：  d+Pmax+1*2
int* Q2;//队列

int pmax23 = 0;

void new_dp23table()
{
	dp23table = new ConDP23[current_config.D + pmax23 + 1];
	Q2 = new int[current_config.D + pmax23 + 1];
	for (int i = 0; i < (current_config.D + pmax23 + 1); i++)
		dp23table[i].x = -1;
}

void free_dptable23()
{
	delete[] dp23table;
	delete[] Q2;
}

int dp23()
{
	memcpy(dp23job_array, job_set, sizeof(job_set));//任务集
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	dp23result = -1;

	//根据LPT规则从大到小排序加工时间
	//sort(dp23job_array, dp23job_array + current_config.n, cmp);
	pmax23 = 0;
	for (int i = 0; i < current_config.n; i++)
	{
		if (pmax23 < dp23job_array[i].proc_time)
			pmax23 = dp23job_array[i].proc_time;
	}
	//创建数组并且初始化
	new_dp23table();

	int m1 = 0, m2 = 0, x = 0;//两台机器
	//初始化第一个数
	m1 = 0;
	m2 = PSUM - m1;
	x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
	dp23table[0].x = x;
	dp23table[0].m1 = "";
	Q2[0] = 0;

	int job_num = 1;//第i个任务
	int q_length = 1;//存储上一次迭代时q里的元素个数
	//开始迭代
	while (job_num <= current_config.n)
	{
		int processtime = dp23job_array[job_num - 1].proc_time;//获取第i个任务的处理时间
		int num = 0;//计数当前任务加了多少次
		//循环Q里的每个元素和第i个任务相加

		for (int i = 0; i < q_length; i++)
		{
			int a = processtime + Q2[i];
			if (a < (current_config.D + pmax23))
			{
				m1 = a;
				m2 = PSUM - a;
				x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
				if (dp23table[a].x == -1)
				{
					dp23table[a].x = x;
					//把加之前的数组都复制过来  
					dp23table[a].m1 = dp23table[Q2[i]].m1;
					//把当前任务加上
					dp23table[a].m1 += to_string(job_num) + " ";//当前处理的任务  任务编号从1开始
					Q2[q_length + num] = a;//q_length从0开始
					num++;
				}
			}
		}

		/*for (int i = 0; i < 200; i++)
			cout << Q2[i] << "  ";
		cout << endl;*/
		q_length += num;
		job_num++;
	}
	int bestresult = 0;//最好的结果的下标
	for (int i = 0; i < current_config.D + pmax23 + 1; i++)
	{
		if (dp23table[i].x > dp23result)
		{
			dp23result = dp23table[i].x;
			bestresult = i;
		}
	}
	m1 = 0, m2 = 0, x = 0;
	//获得了最好的结果的下标  把任务提取出来算一下加工收益
	//cout << endl << "交付期：" << current_config.D << endl;
	m1 = sqlit_sum(dp23table[bestresult].m1);
	m2 = PSUM - m1;
	x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
	QueryPerformanceCounter(&t2);
	dp23_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_dptable23();

	return x;
}
