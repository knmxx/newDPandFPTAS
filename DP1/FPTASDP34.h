#pragma once

#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "job.h"
#include "tool.h"
using namespace std;
//MISHADP+LI  FPTAS
//三台机器
struct Con34
{
	double x;//收益
	string m1;//存储第一台机器加工的任务
	string m2;//存储第二台机器加工的任务
} con34;

double fptas34result = -1;
double fptas34_running_time = -1;
Job fptas34job_array[MAX_N];
Con34** fptas34table;//解集  长度：  d+Pmax+1*2
int** fptasLiQ34;//队列
//Con24 bestresult;
int bestresult34_1;//最好的调度方案的下标
int bestresult34_2;
//int *yuanxulie_state;//存储原序列是否被选中  在最后获得具体调度方案时  选中为1  未选中为0

int newd34 = -1;
double k34 = -1;
double e34 = 0.05;
double a34 = -1;
double psum34 = -1;
int newn34 = -1;//存储新序列的工件个数
int newn34_small = -1;//新序列中小任务的个数

int fptaspmax34 = 0;
double aad34 = -1;//(de r ta)方 * d  每个任务都是这个数的整数倍



void new_fptas34table()
{
	fptas34table = new Con34*[newd34 + fptaspmax34 + 1];
	fptasLiQ34 = new int*[(newd34 + fptaspmax34 + 1)];
	for (int i = 0; i < (newd34 + fptaspmax34 + 1); i++)
	{
		fptas34table[i] = new Con34[newd34 + fptaspmax34 + 1];
	}
	for (int i = 0; i < (newd34 + fptaspmax34 + 1); i++)
	{
		fptasLiQ34[i] = new int[2];
	}
	//yuanxulie_state = new int[current_config.n];
	for (int i = 0; i < current_config.n; i++)
	{
		yuanxulie_state[i] = 0;
	}
}

void free_fptastable34()
{
	for (int i = 0; i < (newd34 + fptaspmax34 + 1); i++)
	{
		delete[] fptas34table[i];
	}
	for (int i = 0; i < (newd34 + fptaspmax34 + 1)*(newd34 + fptaspmax34 + 1); i++)
	{
		delete[] fptasLiQ34[i];
	}
	delete[] fptas34table;
	delete[] fptasLiQ34;
	//delete[] yuanxulie_state;
}

int fptas34()
{
	
	//memcpy(fptas24job_array, job_set, sizeof(job_set));//任务集
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	fptas34result = -1;

	//根据LPT规则从大到小排序加工时间
	//sort(dp23job_array, dp23job_array + current_config.n, cmp);
	a34 = e34 / 10;
	aad34 = a34 * a34 * current_config.D;
	//cout << a24 << endl;

	fptaspmax34 = 0;
	psum34 = 0;
	newn34 = 0;
	double psum34_small_job = 0;
	//计算k 
	//k24 = (1.0 - a24) / (a24*a24);
	//所有任务的处理时间减去a24*d都是a24*a24*d的倍数  那么直接把新序列的处理时间改为他是a24*a24*d的几倍不就好了  处理时间变整数了   
	//cout << "小任务大小为（ad）：" << a24 * current_config.D << endl;
	//cout << "每个处理时间都是aad的整数倍：" << aad24 << endl;
	for (int i = 0; i < current_config.n; i++)
	{
		if (job_set[i].proc_time < (a34*current_config.D))
		{
			//代表是小任务  就加和起来
			psum34_small_job += job_set[i].proc_time;
		}
		else
		{
			newn34++;
			//大任务  每个处理时间都是aad的整数倍
			fptas34job_array[newn34 - 1].proc_time = job_set[i].proc_time*1.0 / aad34;
			fptas34job_array[newn34 - 1].newp = job_set[i].proc_time;//便于追回到原序列  存储的原序列的值
			psum34 += fptas34job_array[newn34 - 1].proc_time*aad34;
			if (fptaspmax34 < fptas34job_array[newn34 - 1].proc_time)
				fptaspmax34 = fptas34job_array[newn34 - 1].proc_time;
		}
	}
	//cout << "xiaorenwu=" << psum24_small_job<<endl;
	//处理小任务  算一下小任务有多少个
	if (psum34_small_job > 0)
	{
		//代表有小任务
		if ((int)(psum34_small_job / (a34*current_config.D)) == 0)
			newn34_small = 1;
		else
			newn34_small = (int)(psum34_small_job / (a34*current_config.D)) + 1;//7/6=两个任务  所以加一
	}
	//newn24 += newn24_small;
	//将小任务加到序列中
	for (int i = 0; i < newn34_small; i++)
	{
		//最大的任务不会在小任务里产生  就不用算进去了
		newn34++;
		fptas34job_array[newn34 - 1].proc_time = 1.0 / a34;//小任务是aad的1/a倍  是个整数
		psum34 += fptas34job_array[newn34 - 1].proc_time*aad34;
		fptas34job_array[newn34 - 1].newp = 0;//可以判断是不是小任务
	}
	////输出所有新的序列
	//for (int i = 0; i < newn24; i++)
	//{
	//	cout << fptas24job_array[i].proc_time*aad24 <<endl;
	//}


	//cout << endl<<"k====="<<k24<<endl;
	//cout << endl << "交付期=====" << current_config.D << endl;
	//reset加工时间及交付期
	/*cout << endl << "重新设置的处理时间=====";*/
	//newd24 = psum24/ current_config.m;
	//newd24 = current_config.D;
	newd34 = current_config.D / aad34;
	//cout << endl << "psum=====" << psum23 << endl;
	//cout << endl << "newd24=====" << newd24 << endl;
	//创建数组并且初始化
	new_fptas34table();

	
	double m1 = 0, m2 = 0, m3 = 0, x = 0;//两台机器
	//初始化第一个数
	m1 = 0;
	m2 = 0;
	m3 = psum34 - m1 - m2;
	x = ((m1 > current_config.D) ? current_config.D : m1)
		+ ((m2 > current_config.D) ? current_config.D : m2)
		+ ((m3 > current_config.D) ? current_config.D : m3);
	fptas34table[0][0].x = x;
	fptas34table[0][0].m1 = "";
	fptas34table[0][0].m2 = "";
	fptasLiQ34[0][0] = m1;
	fptasLiQ34[0][1] = m2;
	bestresult34_1 = 0;
	bestresult34_2 = 0;
	int job_num = 1;//第i个任务
	int q_length = 1;//存储上一次迭代时q里的元素个数
	//开始迭代
	while (job_num <= newn34)
	{
		int processtime = fptas34job_array[job_num - 1].proc_time;//获取第i个任务的处理时间
		int num = 0;//计数当前任务加了多少次
		//循环Q里的每个元素和第i个任务相加

		for (int i = 0; i < q_length; i++)
		{
			
			int a = 0, b = 0;
			//当第i个任务在第一个机器上加工
			a = processtime + fptasLiQ34[i][0];
			b = fptasLiQ34[i][1];
			if (a < (newd34 + fptaspmax34) && b < (newd34 + fptaspmax34))
			{
				m1 = a * aad34;
				m2 = b * aad34;
				m3 = psum34 - m1 - m2;
				x = ((m1 > current_config.D) ? current_config.D : m1) 
					+ ((m2 > current_config.D) ? current_config.D : m2)
					+ ((m3 > current_config.D) ? current_config.D : m3);
				//cout << "x===" << x << endl;
				//cout << fptas24table[a].x << endl;
				if (fptas34table[a][b].x == 0)
				{
					//往m1上加任务
					fptas34table[a][b].x = x;
					//把加之前的数组都复制过来  
					fptas34table[a][b].m1 = fptas34table[fptasLiQ34[i][0]][fptasLiQ34[i][1]].m1;
					fptas34table[a][b].m2 = fptas34table[fptasLiQ34[i][0]][fptasLiQ34[i][1]].m2;
					//把当前任务加上
					fptas34table[a][b].m1 += to_string(job_num) + " ";//当前处理的任务  任务编号从1开始
					fptasLiQ34[q_length + num][0] = a;//q_length从0开始
					fptasLiQ34[q_length + num][1] = b;//q_length从0开始
					num++;
					if (fptas34table[bestresult34_1][bestresult34_2].x < x)
					{
						bestresult34_1 = a;
						bestresult34_2 = b;
					}
				}
			}

			//当第i个任务在第二个机器上加工
			a = fptasLiQ34[i][0];
			b = processtime + fptasLiQ34[i][1];
			if (a < (newd34 + fptaspmax34) && b < (newd34 + fptaspmax34))
			{
				m1 = a * aad34;
				m2 = b * aad34;
				m3 = psum34 - m1 - m2;
				x = ((m1 > current_config.D) ? current_config.D : m1)
					+ ((m2 > current_config.D) ? current_config.D : m2)
					+ ((m3 > current_config.D) ? current_config.D : m3);
				//cout << "x===" << x << endl;
				//cout << fptas24table[a].x << endl;
				if (fptas34table[a][b].x == 0)
				{
					//往m2上加任务
					fptas34table[a][b].x = x;
					//把加之前的数组都复制过来  
					fptas34table[a][b].m1 = fptas34table[fptasLiQ34[i][0]][fptasLiQ34[i][1]].m1;
					fptas34table[a][b].m2 = fptas34table[fptasLiQ34[i][0]][fptasLiQ34[i][1]].m2;
					//把当前任务加上
					fptas34table[a][b].m2 += to_string(job_num) + " ";//当前处理的任务  任务编号从1开始
					fptasLiQ34[q_length + num][0] = a;//q_length从0开始
					fptasLiQ34[q_length + num][1] = b;//q_length从0开始
					num++;
					if (fptas34table[bestresult34_1][bestresult34_2].x < x)
					{
						bestresult34_1 = a;
						bestresult34_2 = b;
					}
				}
			}
		}

		//for (int i = 0; i < 200; i++)
		//	cout << fptasLinQ24[i] << "  ";
		//cout << endl;
		q_length += num;
		job_num++;
	}
	m1 = 0, m2 = 0, m3 = 0, x = 0;
	//获得了最好的结果的下标  把任务提取出来算一下加工收益
	//cout << endl << "交付期：" << current_config.D << endl;
	//cout << "fptas24table[bestresult].m1" << fptas24table[bestresult].m1 << endl;
	m1 = sqlit_sum_li_n(fptas34table[bestresult34_1][bestresult34_2].m1, fptas34job_array, a34);
	m2 = sqlit_sum_li_n(fptas34table[bestresult34_1][bestresult34_2].m2, fptas34job_array, a34);
	m3 = PSUM - m1 - m2;
	/*cout << "m1=" << m1 << endl;
	cout << "m2=" << m2 << endl;*/
	x = ((m1 > current_config.D) ? current_config.D : m1) 
		+ ((m2 > current_config.D) ? current_config.D : m2)
		+ ((m3 > current_config.D) ? current_config.D : m3);
	//cout << endl << "fptas的结果：" << fptas23result << "    xxx=" << xxx << endl;
	QueryPerformanceCounter(&t2);
	fptas34_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_fptastable34();

	return x;

}

