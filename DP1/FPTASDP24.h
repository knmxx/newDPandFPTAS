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
//两台机器
//这个版本是数组存得是多少倍  即FPTASDP242
struct Con24
{
	double x=0;//收益
	string m1;//M1上的加工负载
} con24;

double fptas24result = -1;
double fptas24_running_time = -1;
Job fptas24job_array[MAX_N];
Con24* fptas24table;//解集  长度：  d+Pmax+1*2
int* fptasLiQ24;//队列
//Con24 bestresult;
int bestresult24;//最好的调度方案的下标

int newd24 = -1;
double k24 = -1;
double e24 = 0.075;
double a24 = -1;
double psum24 = -1;
int newn24 = -1;//存储新序列的工件个数
int newn24_small = -1;//新序列中小任务的个数

int fptaspmax24 = 0;
double aad24 = -1;//(de r ta)方 * d  每个任务都是这个数的整数倍



void new_fptas24table()
{
	fptas24table = new Con24[newd24 + fptaspmax24 + 1];
	fptasLiQ24 = new int[newd24 + fptaspmax24 + 1];
}

void free_fptastable24()
{
	delete[] fptas24table;
	delete[] fptasLiQ24;
}

int fptas24()
{
	//memcpy(fptas24job_array, job_set, sizeof(job_set));//任务集
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	fptas24result = -1;

	//根据LPT规则从大到小排序加工时间
	//sort(dp23job_array, dp23job_array + current_config.n, cmp);
	a24 = e24 / 10;
	aad24 = a24 * a24 * current_config.D;
	//cout << a24 << endl;
	
	fptaspmax24 = 0;
	psum24 = 0;
	newn24 = 0;
	double psum24_small_job = 0;
	//计算k 
	//k24 = (1.0 - a24) / (a24*a24);
	//所有任务的处理时间减去a24*d都是a24*a24*d的倍数  那么直接把新序列的处理时间改为他是a24*a24*d的几倍不就好了  处理时间变整数了   
	//cout << "小任务大小为（ad）：" << a24 * current_config.D << endl;
	//cout << "每个处理时间都是aad的整数倍：" << aad24 << endl;
	for (int i = 0; i < current_config.n; i++)
	{
		if (job_set[i].proc_time < (a24*current_config.D))
		{
			//代表是小任务  就加和起来
			psum24_small_job += job_set[i].proc_time;
		}
		else
		{
			newn24++;
			//大任务  每个处理时间都是aad的整数倍  
			fptas24job_array[newn24 - 1].proc_time = job_set[i].proc_time*1.0 / aad24;
			fptas24job_array[newn24 - 1].newp = job_set[i].proc_time;//便于追回到原序列  存储的原序列的值
			psum24 += fptas24job_array[newn24 - 1].proc_time*aad24;
			if (fptaspmax24 < fptas24job_array[newn24 - 1].proc_time)
				fptaspmax24 = fptas24job_array[newn24 - 1].proc_time;
		}
	}
	//cout << "xiaorenwu=" << psum24_small_job<<endl;
	//处理小任务  算一下小任务有多少个
	if (psum24_small_job > 0)
	{
		//代表有小任务
		if ((int)(psum24_small_job / (a24*current_config.D)) == 0)
			newn24_small = 1;
		else
			newn24_small = (int)(psum24_small_job / (a24*current_config.D)) + 1;//7/6=两个任务  所以加一
	}
	//newn24 += newn24_small;
	//将小任务加到序列中
	for (int i = 0; i < newn24_small; i++)
	{
		//最大的任务不会在小任务里产生  就不用算进去了 ***如果都是小任务  那么最大处理时间就是小任务大小
		newn24++;
		fptas24job_array[newn24 - 1].proc_time = 1.0 / a24;//小任务是aad的1/a倍  是个整数
		psum24 += fptas24job_array[newn24 - 1].proc_time*aad24;
		fptas24job_array[newn24 - 1].newp = 0;//可以判断是不是小任务
	}
	if (fptaspmax24 < (int)(1.0 / a24))
		fptaspmax24 = (int)(1.0 / a24);
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
	newd24 = current_config.D / aad24;
	//cout << endl << "psum=====" << psum23 << endl;
	//cout << endl << "newd24=====" << newd24 << endl;
	//创建数组并且初始化
	new_fptas24table();


	double m1 = 0, m2 = 0, x = 0;//两台机器
	//初始化第一个数
	m1 = 0;
	m2 = psum24 - m1;
	x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
	fptas24table[0].x = x;
	fptas24table[0].m1 = "";
	fptasLiQ24[0] = 0;
	bestresult24 = 0;
	int job_num = 1;//第i个任务
	int q_length = 1;//存储上一次迭代时q里的元素个数
	//开始迭代
	while (job_num <= newn24)
	{
		int processtime = fptas24job_array[job_num - 1].proc_time;//获取第i个任务的处理时间
		int num = 0;//计数当前任务加了多少次
		//循环Q里的每个元素和第i个任务相加

		for (int i = 0; i < q_length; i++)
		{
			int a = processtime + fptasLiQ24[i];
			if (a < (newd24 + fptaspmax24))
			{
				m1 = a * aad24;
				m2 = psum24 - m1;
				x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
				//cout << "x===" << x << endl;
				//cout << fptas24table[a].x << endl;
				if (fptas24table[a].x == 0)
				{
					fptas24table[a].x = x;
					//把加之前的数组都复制过来  
					fptas24table[a].m1 = fptas24table[fptasLiQ24[i]].m1;
					//把当前任务加上
					fptas24table[a].m1 += to_string(job_num) + " ";//当前处理的任务  任务编号从1开始
					fptasLiQ24[q_length + num] = a;//q_length从0开始
					num++;
					if (fptas24table[bestresult24].x < x)
					{
						bestresult24 = a;
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
	m1 = 0, m2 = 0, x = 0;
	//获得了最好的结果的下标  把任务提取出来算一下加工收益
	//cout << endl << "交付期：" << current_config.D << endl;
	//cout << "fptas24table[bestresult].m1" << fptas24table[bestresult].m1 << endl;
	m1 = sqlit_sum_li_2(fptas24table[bestresult24].m1, fptas24job_array, a24);
	m2 = PSUM - m1;
	/*cout << "m1=" << m1 << endl;
	cout << "m2=" << m2 << endl;*/
	x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
	//cout << endl << "fptas的结果：" << fptas23result << "    xxx=" << xxx << endl;
	QueryPerformanceCounter(&t2);
	fptas24_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_fptastable24();

	return x;

}

