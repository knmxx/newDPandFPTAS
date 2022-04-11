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
//三台机器
struct Con33
{
	int x;//收益
	string m1 = "";//存储第一台机器加工的任务
	string m2 = "";//存储第二台机器加工的任务
} con33;

int fptas33result = -1;
double fptas33_running_time = -1;
Job fptas33job_array[MAX_N];
Con33** fptas33table;//解集  长度：  d+Pmax+1*2
int** fptasQ3;//队列

int newd33 = -1;
double k33 = -1;
double e33 = 0.05;
int psum33 = -1;

int fptaspmax33 = 0;



void new_fptas33table()
{

	fptas33table = new Con33*[newd33 +  fptaspmax33 + 1];
	fptasQ3 = new int*[(newd33 + fptaspmax33 + 1)*(newd33 + fptaspmax33 + 1)];
	for (int i = 0; i < (newd33 + fptaspmax33 + 1); i++)
	{
		fptas33table[i] = new Con33[newd33 + fptaspmax33 + 1];
	}
	for (int i = 0; i < (newd33 + fptaspmax33 + 1)*(newd33 + fptaspmax33 + 1); i++)
	{
		fptasQ3[i] = new int[2];
	}
	for (int i = 0; i < (newd33 + fptaspmax33 + 1); i++)
		for (int j = 0; j < (newd33 + fptaspmax33 + 1); j++)
			fptas33table[i][j].x = -1;
}

void free_fptastable33()
{
	for (int i = 0; i < (newd33 + fptaspmax33 + 1); i++)
	{
		delete[] fptas33table[i];
	}
	for (int i = 0; i < (newd33 + fptaspmax33 + 1)*(newd33 + fptaspmax33 + 1); i++)
	{
		delete[] fptasQ3[i];
	}
	delete[] fptas33table;
	delete[] fptasQ3;
}

int fptas33()
{
	memcpy(fptas33job_array, job_set, sizeof(job_set));//任务集
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	fptas33result = -1;

	//根据LPT规则从大到小排序加工时间
	//sort(dp33job_array, dp33job_array + current_config.n, cmp);
	fptaspmax33 = 0;
	psum33 = 0;
	//计算k 
	k33 = (e33*current_config.D) / ((current_config.m - 1)*current_config.n);
	//cout << endl << "k=====" << k33 << endl;
	if (k33 <= 1)
	{
		k33 = 1.0;
	}
	
	//reset加工时间及交付期
	/*cout << endl << "重新设置的处理时间=====";*/
	for (int i = 0; i < current_config.n; i++)
	{
		fptas33job_array[i].proc_time = (fptas33job_array[i].proc_time*1.0) / k33;
		/* cout<<endl<< fptas23job_array[i].proc_time<<endl;*/
		psum33 += fptas33job_array[i].proc_time;
		if (fptaspmax33 < fptas33job_array[i].proc_time)
			fptaspmax33 = fptas33job_array[i].proc_time;
	}
	newd33 = (current_config.D*1.0) / k33;
	/*cout << endl << "psum=====" << psum23 << endl;
	cout << endl << "newd23=====" << newd23 << endl;*/
	//创建数组并且初始化
	new_fptas33table();

	int m1 = 0, m2 = 0, m3 = 0, x = 0;//三台机器
	//初始化第一个数
	m1 = 0;
	m2 = 0;
	m3 = psum33 - m1 - m2;
	x = ((m1 > newd33) ? newd33 : m1) + ((m2 > newd33) ? newd33 : m2) + ((m3 > newd33) ? newd33 : m3);
	fptas33table[0][0].x = x;
	fptas33table[0][0].m1 = "";
	fptas33table[0][0].m2 = "";
	fptasQ3[0][0] = m1;
	fptasQ3[0][1] = m2;

	int job_num = 1;//第i个任务
	int q_length = 1;//存储上一次迭代时q里的元素个数
	//开始迭代
	while (job_num <= current_config.n)
	{
		int processtime = fptas33job_array[job_num - 1].proc_time;//获取第i个任务的处理时间
		int num = 0;//计数当前任务加了多少次
		//循环Q里的每个元素和第i个任务相加

		for (int i = 0; i < q_length; i++)
		{
			int a = 0, b = 0;
			//当第i个任务在第一个机器上加工
			a = processtime + fptasQ3[i][0];
			b = fptasQ3[i][1];
			if (a < (newd33 + fptaspmax33) && b < (newd33 + fptaspmax33))
			{
				m1 = a;
				m2 = b;
				m3 = psum33 - a - b;
				x = ((m1 > newd33) ? newd33 : m1) + ((m2 > newd33) ? newd33 : m2) + ((m3 > newd33) ? newd33 : m3);
				if (fptas33table[a][b].x == -1)
				{
					//往m1上加任务
					fptas33table[a][b].x = x;
					//把加之前的数组都复制过来  包括m1和m2的
					fptas33table[a][b].m1 = fptas33table[fptasQ3[i][0]][fptasQ3[i][1]].m1;
					fptas33table[a][b].m2 = fptas33table[fptasQ3[i][0]][fptasQ3[i][1]].m2;
					//把当前任务加到m1上
					fptas33table[a][b].m1 += to_string(job_num)+" ";//当前处理的任务  任务编号从1开始
					fptasQ3[q_length + num][0] = a;//q_length从0开始
					fptasQ3[q_length + num][1] = b;//q_length从0开始
					num++;
				}
			}
			//当第i个任务在第二个机器上加工
			a = fptasQ3[i][0];
			b = processtime + fptasQ3[i][1];
			if (a < (newd33 + fptaspmax33) && b < (newd33 + fptaspmax33))
			{
				m1 = a;
				m2 = b;
				m3 = psum33 - a - b;
				x = ((m1 > newd33) ? newd33 : m1) + ((m2 > newd33) ? newd33 : m2) + ((m3 > newd33) ? newd33 : m3);
				if (fptas33table[a][b].x == -1)
				{
					//往m2上加任务
					fptas33table[a][b].x = x;
					//把加之前的数组都复制过来 包括m1,m2的
					fptas33table[a][b].m1 = fptas33table[fptasQ3[i][0]][fptasQ3[i][1]].m1;
					fptas33table[a][b].m2 = fptas33table[fptasQ3[i][0]][fptasQ3[i][1]].m2;
					//把当前任务加到m2上
					fptas33table[a][b].m2 += to_string(job_num) +" ";//当前处理的任务  任务编号从1开始
					fptasQ3[q_length + num][0] = a;//q_length从0开始
					fptasQ3[q_length + num][1] = b;//q_length从0开始
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
	int bestresult_1 = 0, bestresult_2 = 0;//最好的结果的下标  二维数组  俩下标
	for (int i = 0; i < newd33 + fptaspmax33 + 1; i++)
	{
		for (int j = 0; j < newd33 + fptaspmax33 + 1; j++)
		{
			if (fptas33table[i][j].x > fptas33result)
			{
				fptas33result = fptas33table[i][j].x;
				bestresult_1 = i;
				bestresult_2 = j;
			}
		}
	}
	m1 = 0, m2 = 0, m3 = 0, x = 0;
	//获得了最好的结果的下标  把任务提取出来算一下加工收益
	//cout << endl << "交付期：" << current_config.D << endl;
	//cout << fptas33table[bestresult_1][bestresult_2].m1  ;
	m1 = sqlit_sum(fptas33table[bestresult_1][bestresult_2].m1);
	m2 = sqlit_sum(fptas33table[bestresult_1][bestresult_2].m2);
	m3 = PSUM - m1 - m2;
	x = ((m1 > current_config.D) ? current_config.D : m1) +
		((m2 > current_config.D) ? current_config.D : m2) +
		((m3 > current_config.D) ? current_config.D : m3);
	//cout << endl << "fptas的结果：" << fptas33result << "    xxx=" << xxx << endl;
	QueryPerformanceCounter(&t2);
	fptas33_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_fptastable33();

	return x;
}
