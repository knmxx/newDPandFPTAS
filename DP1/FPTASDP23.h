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
//��̨����
struct Con23
{
	int x;//����
	string m1 = "";//M1�ϵļӹ�����
} con23;

int fptas23result = -1;
double fptas23_running_time = -1;
Job fptas23job_array[MAX_N];
Con23* fptas23table;//�⼯  ���ȣ�  d+Pmax+1*2
int* fptasQ2;//����

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
	memcpy(fptas23job_array, job_set, sizeof(job_set));//����
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	fptas23result = -1;

	//����LPT����Ӵ�С����ӹ�ʱ��
	//sort(dp23job_array, dp23job_array + current_config.n, cmp);
	fptaspmax23 = 0;
	psum23 = 0;
	//����k 
	k23 = (e23*current_config.D) / ((current_config.m - 1)*current_config.n);
	if (k23 <= 1)
	{
		k23 = 1.0;
	}
	/*cout << endl<<"k====="<<k23<<endl;*/
	//reset�ӹ�ʱ�估������
	/*cout << endl << "�������õĴ���ʱ��=====";*/
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
	//�������鲢�ҳ�ʼ��
	new_fptas23table();

	int m1 = 0, m2 = 0, x = 0;//��̨����
	//��ʼ����һ����
	m1 = 0;
	m2 = psum23 - m1;
	x = ((m1 > newd23) ? newd23 : m1) + ((m2 > newd23) ? newd23 : m2);
	fptas23table[0].x = x;
	fptas23table[0].m1 = "";
	fptasQ2[0] = 0;

	int job_num = 1;//��i������
	int q_length = 1;//�洢��һ�ε���ʱq���Ԫ�ظ���
	//��ʼ����
	while (job_num <= current_config.n)
	{
		int processtime = fptas23job_array[job_num - 1].proc_time;//��ȡ��i������Ĵ���ʱ��
		int num = 0;//������ǰ������˶��ٴ�
		//ѭ��Q���ÿ��Ԫ�غ͵�i���������

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
					//�Ѽ�֮ǰ�����鶼���ƹ���  
					fptas23table[a].m1 = fptas23table[fptasQ2[i]].m1;
					//�ѵ�ǰ�������
					fptas23table[a].m1 += to_string(job_num) + " ";//��ǰ���������  �����Ŵ�1��ʼ
					fptasQ2[q_length + num] = a;//q_length��0��ʼ
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
	int bestresult = 0;//��õĽ�����±�
	for (int i = 0; i < newd23 + fptaspmax23 + 1; i++)
	{
		if (fptas23table[i].x > fptas23result)
		{
			fptas23result = fptas23table[i].x;
			bestresult = i;
		}
	}
	m1 = 0, m2 = 0, x = 0;
	//�������õĽ�����±�  ��������ȡ������һ�¼ӹ�����
	//cout << endl << "�����ڣ�" << current_config.D << endl;
	m1 = sqlit_sum(fptas23table[bestresult].m1);
	m2 = PSUM - m1;
	x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
	//cout << endl << "fptas�Ľ����" << fptas23result << "    xxx=" << xxx << endl;
	QueryPerformanceCounter(&t2);
	fptas23_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_fptastable23();

	return x;
}

