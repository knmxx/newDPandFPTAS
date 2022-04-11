#pragma once

#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "job.h"
#include "tool.h"
using namespace std;
//MISHA  FPTAS
//两台机器
struct Con23
{
	int x;//收益
	string m1 = "";//M1上的加工负载
} con23;

int fptas23result = -1;
double fptas23_running_time = -1;
Job fptas23job_array[MAX_N];
Con23* fptas23table;//解集  长度：  d+Pmax+1*2
int* fptasQ2;//队列

int newd23 = -1;
double k23 = -1;
double e23 = 0.05;
int psum23 = -1;

int fptaspmax23 = 0;



void new_fptas23table()
{
	fptas23table = new Con23[newd23 + fptaspmax23 + 1];
	fptasQ2 = new int[newd23 + fptaspmax23 + 1];
	for (int i = 0; i < (newd23 + fptaspmax23 + 1); i++)
		fptas23table[i].x = -1;
}

void free_fptastable23()
{
	delete[] fptas23table;
	delete[] fptasQ2;
}

int fptas23()
{
	memcpy(fptas23job_array, job_set, sizeof(job_set));//任务集
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	fptas23result = -1;

	//根据LPT规则从大到小排序加工时间
	//sort(dp23job_array, dp23job_array + current_config.n, cmp);
	fptaspmax23 = 0;
	psum23 = 0;
	//计算k 
	k23 = (e23*current_config.D) / ((current_config.m - 1)*current_config.n);
	if (k23 <= 1)
	{
		k23 = 1.0;
	}
	/*cout << endl<<"k====="<<k23<<endl;*/
	//reset加工时间及交付期
	/*cout << endl << "重新设置的处理时间=====";*/
	for (int i = 0; i < current_config.n; i++)
	{
		fptas23job_array[i].proc_time = (fptas23job_array[i].proc_time*1.0) / k23;
		//cout<<fptas23job_array[i].proc_time<<endl;
		psum23 += fptas23job_array[i].proc_time;
		if (fptaspmax23 < fptas23job_array[i].proc_time)
			fptaspmax23 = fptas23job_array[i].proc_time;
	}
	newd23 = (current_config.D*1.0) / k23;
	/*cout << endl << "psum=====" << psum23 << endl;
	cout << endl << "newd23=====" << newd23 << endl;*/
	//创建数组并且初始化
	new_fptas23table();

	int m1 = 0, m2 = 0, x = 0;//两台机器
	//初始化第一个数
	m1 = 0;
	m2 = psum23 - m1;
	x = ((m1 > newd23) ? newd23 : m1) + ((m2 > newd23) ? newd23 : m2);
	fptas23table[0].x = x;
	fptas23table[0].m1 = "";
	fptasQ2[0] = 0;

	int job_num = 1;//第i个任务
	int q_length = 1;//存储上一次迭代时q里的元素个数
	//开始迭代
	while (job_num <= current_config.n)
	{
		int processtime = fptas23job_array[job_num - 1].proc_time;//获取第i个任务的处理时间
		int num = 0;//计数当前任务加了多少次
		//循环Q里的每个元素和第i个任务相加

		for (int i = 0; i < q_length; i++)
		{
			int a = processtime + fptasQ2[i];
			if (a < (newd23 + fptaspmax23))
			{
				m1 = a;
				m2 = psum23 - a;
				x = ((m1 > newd23) ? newd23 : m1) + ((m2 > newd23) ? newd23 : m2);
				if (fptas23table[a].x == -1)
				{
					fptas23table[a].x = x;
					//把加之前的数组都复制过来  
					fptas23table[a].m1 = fptas23table[fptasQ2[i]].m1;
					//把当前任务加上
					fptas23table[a].m1 += to_string(job_num) + " ";//当前处理的任务  任务编号从1开始
					fptasQ2[q_length + num] = a;//q_length从0开始
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
	for (int i = 0; i < newd23 + fptaspmax23 + 1; i++)
	{
		if (fptas23table[i].x > fptas23result)
		{
			fptas23result = fptas23table[i].x;
			bestresult = i;
		}
	}
	m1 = 0, m2 = 0, x = 0;
	//获得了最好的结果的下标  把任务提取出来算一下加工收益
	//cout << endl << "交付期：" << current_config.D << endl;
	m1 = sqlit_sum(fptas23table[bestresult].m1);
	m2 = PSUM - m1;
	x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
	//cout << endl << "fptas的结果：" << fptas23result << "    xxx=" << xxx << endl;
	QueryPerformanceCounter(&t2);
	fptas23_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_fptastable23();

	return x;
}

