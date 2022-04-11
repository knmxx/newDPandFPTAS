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
//四台机器
struct Con43
{
	int x;//收益
	string m1 = "";//存储第一台机器加工的任务
	string m2 = "";//存储第二台机器加工的任务
	string m3 = "";//存储第三台机器加工的任务
} con43;

int fptas43result = -1;
double fptas43_running_time = -1;
Job fptas43job_array[MAX_N];
Con43*** fptas43table;//解集  长度：  d+Pmax+1*2
int** fptasQ4;//队列

int newd43 = -1;
double k43 = -1;
double e43 = 0.05;
int psum43 = -1;

int fptaspmax43 = 0;

int num43 = 0;//计数当前任务加了多少次
int q_length43 = 1;//存储上一次迭代时q里的元素个数
int job_num43 = 1;//第i个任务
//
//试一下  计算循环的次数变为一个值
int tablelength43 = 0;
int q4length = 0;

void new_fptas43table()
{

	fptas43table = new Con43**[tablelength43];
	fptasQ4 = new int*[q4length];
	for (int i = 0; i < tablelength43; i++)
	{
		fptas43table[i] = new Con43*[tablelength43];
	}
	for (int i = 0; i < tablelength43; i++)
	{
		for (int j = 0; j < tablelength43; j++)
		{
			fptas43table[i][j] = new Con43[tablelength43];
		}
	}
	for (int i = 0; i < q4length; i++)
	{
		fptasQ4[i] = new int[3];
	}
	for (int i = 0; i < tablelength43; i++)
		for (int j = 0; j < tablelength43; j++)
			for (int k = 0; k < tablelength43; k++)
				fptas43table[i][j][k].x = -1;
	for (int i = 0; i < q4length; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fptasQ4[i][j] = -1;
		}
	}
}

void free_fptastable43()
{
	for (int i = 0; i < tablelength43; i++)
	{
		for (int j = 0; j < tablelength43; j++)
		{
			delete[] fptas43table[i][j];
		}
		delete[] fptas43table[i];
	}
	for (int i = 0; i < q4length; i++)
	{
		delete[] fptasQ4[i];
	}
	delete[] fptas43table;
	delete[] fptasQ4;
}

//新的数组下标：a，b，c   加的数组下标i，j，k  加到第m台机器上  m从1开始 
void dispatch43(int a, int b, int c,int i,int j,int k,int m)//分配机器   
{
	int m1 = 0, m2 = 0, m3 = 0, m4 = 0, x = 0;//四台机器11
	if (a < (newd43 + fptaspmax43) && b < (newd43 + fptaspmax43) && c < (newd43 + fptaspmax43))
	{
		m1 = a;
		m2 = b;
		m3 = c;
		m4 = psum43 - a - b - c;
		//cout << "负载：" << m1 << "  " << m2 << "  " << m3 << "  " << m4 << endl;
		x = ((m1 > newd43) ? newd43 : m1) +
			((m2 > newd43) ? newd43 : m2) +
			((m3 > newd43) ? newd43 : m3) +
			((m4 > newd43) ? newd43 : m4);
		if (fptas43table[a][b][c].x == -1 && fptas43table[a][c][b].x == -1 && 
			fptas43table[b][a][c].x == -1 && fptas43table[b][c][a].x == -1 && 
			fptas43table[c][a][b].x == -1 && fptas43table[c][b][a].x == -1)
		{
			fptas43table[a][b][c].x = x;
			//把加之前的数组都复制过来  包括m1和m2的
			fptas43table[a][b][c].m1 =
				fptas43table[i][j][k].m1;
			fptas43table[a][b][c].m2 =
				fptas43table[i][j][k].m2;
			fptas43table[a][b][c].m3 =
				fptas43table[i][j][k].m3;
			switch (m) {
			case 1:
				//把当前任务加到m1上
				fptas43table[a][b][c].m1 += to_string(job_num43) + " ";//当前处理的任务  任务编号从1开始
				break;
			case 2:
				//把当前任务加到m2上
				fptas43table[a][b][c].m2 += to_string(job_num43) + " ";//当前处理的任务  任务编号从1开始
				break;
			case 3:
				//把当前任务加到m3上
				fptas43table[a][b][c].m3 += to_string(job_num43) + " ";//当前处理的任务  任务编号从1开始
				break;
			}
			fptasQ4[q_length43 + num43][0] = a;//q_length从0开始
			fptasQ4[q_length43 + num43][1] = b;//q_length从0开始
			fptasQ4[q_length43 + num43][2] = c;//q_length从0开始
			num43++;
		}
	}
}

int fptas43()
{
	memcpy(fptas43job_array, job_set, sizeof(job_set));//任务集
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	fptas43result = -1;

	//根据LPT规则从大到小排序加工时间
	//sort(dp43job_array, dp43job_array + current_config.n, cmp);
	fptaspmax43 = 0;
	psum43 = 0;
	//计算k 
	k43 = (e43*current_config.D) / ((current_config.m - 1)*current_config.n);
	if (k43 <= 1)
	{
		k43 = 1.0;
	}
	//cout << endl<<"k====="<<k43<<endl;
	//reset加工时间及交付期
	/*cout << endl << "重新设置的处理时间=====";*/
	for (int i = 0; i < current_config.n; i++)
	{
		fptas43job_array[i].proc_time = (fptas43job_array[i].proc_time*1.0) / k43;
		/* cout<<endl<< fptas23job_array[i].proc_time<<endl;*/
		psum43 += fptas43job_array[i].proc_time;
		if (fptaspmax43 < fptas43job_array[i].proc_time)
			fptaspmax43 = fptas43job_array[i].proc_time;
	}
	newd43 = (current_config.D*1.0) / k43;
	//cout << endl << "psum=====" << psum43 << endl;
	//cout << endl << "newd43=====" << newd43 << endl;
	//test
	q4length = (newd43 + fptaspmax43 + 1)*(newd43 + fptaspmax43 + 1)*(newd43 + fptaspmax43 + 1);
	tablelength43 = newd43 + fptaspmax43 + 1;
	//创建数组并且初始化
	new_fptas43table();

	


	int m1 = 0, m2 = 0, m3 = 0, m4 = 0, x = 0;//四台机器
	//初始化第一个数
	m1 = 0;
	m2 = 0;
	m3 = 0;
	m4 = psum43 - m1 - m2 - m3;
	x = ((m1 > newd43) ? newd43 : m1) + 
		((m2 > newd43) ? newd43 : m2) + 
		((m3 > newd43) ? newd43 : m3) +
		((m4 > newd43) ? newd43 : m4);
	fptas43table[0][0][0].x = x;
	fptas43table[0][0][0].m1 = "";
	fptas43table[0][0][0].m2 = "";
	fptas43table[0][0][0].m3 = "";
	fptasQ4[0][0] = m1;
	fptasQ4[0][1] = m2;
	fptasQ4[0][2] = m3;

	q_length43 = 1;
	job_num43 = 1;//第i个任务
	//开始迭代
	while (job_num43 <= current_config.n)
	{
		int processtime = fptas43job_array[job_num43 - 1].proc_time;//获取第i个任务的处理时间
		num43 = 0;//计数当前任务加了多少次
		//循环Q里的每个元素和第i个任务相加

		for (int i = 0; i < q_length43; i++)
		{
			int a = 0, b = 0, c = 0;
			//当第i个任务在第一个机器上加工
			a = processtime + fptasQ4[i][0];
			b = fptasQ4[i][1];
			c = fptasQ4[i][2];
			dispatch43(a, b, c, fptasQ4[i][0], fptasQ4[i][1], fptasQ4[i][2], 1);
			//当第i个任务在第二个机器上加工
			a = fptasQ4[i][0];
			b = processtime + fptasQ4[i][1];
			c = fptasQ4[i][2];
			dispatch43(a, b, c, fptasQ4[i][0], fptasQ4[i][1], fptasQ4[i][2], 2);
			//当第i个任务在第三个机器上加工
			a = fptasQ4[i][0];
			b = fptasQ4[i][1];
			c = processtime + fptasQ4[i][2];
			dispatch43(a, b, c, fptasQ4[i][0], fptasQ4[i][1], fptasQ4[i][2], 3);
		}

		/*for (int i = 0; i < 200; i++)
			cout << Q2[i] << "  ";
		cout << endl;*/
		q_length43 += num43;
		job_num43++;
	}
	int bestresult_1 = 0, bestresult_2 = 0, bestresult_3 = 0;//最好的结果的下标  三维数组  三下标
	for (int i = 0; i < tablelength43; i++)
	{
		for (int j = 0; j < tablelength43; j++)
		{
			for (int k = 0; k < tablelength43; k++)
			{
				if (fptas43table[i][j][k].x > fptas43result)
				{
					fptas43result = fptas43table[i][j][k].x;
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
	//cout << fptas43table[bestresult_1][bestresult_2][bestresult_3].m1 << endl;
	//cout << fptas43table[bestresult_1][bestresult_2][bestresult_3].m2 << endl;
	//cout << fptas43table[bestresult_1][bestresult_2][bestresult_3].m3 << endl;
	m1 = sqlit_sum(fptas43table[bestresult_1][bestresult_2][bestresult_3].m1);
	m2 = sqlit_sum(fptas43table[bestresult_1][bestresult_2][bestresult_3].m2);
	m3 = sqlit_sum(fptas43table[bestresult_1][bestresult_2][bestresult_3].m3);
	m4 = PSUM - m1 - m2 - m3;
	x = ((m1 > current_config.D) ? current_config.D : m1) +
		((m2 > current_config.D) ? current_config.D : m2) +
		((m3 > current_config.D) ? current_config.D : m3) +
		((m4 > current_config.D) ? current_config.D : m4);
	//cout << endl << "fptas的结果：" << fptas33result << "    xxx=" << xxx << endl;
	QueryPerformanceCounter(&t2);
	fptas43_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_fptastable43();

	return x;
}
