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
//ȥ��ѭ������������ȷ���
struct ConDP23
{
	int x;//����
	string m1 = "";//M1�ϵļӹ�����
} condp23;


int dp23result = -1;
double dp23_running_time = -1;
Job dp23job_array[MAX_N];
ConDP23* dp23table;//�⼯  ���ȣ�  d+Pmax+1*2
int* Q2;//����

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
	memcpy(dp23job_array, job_set, sizeof(job_set));//����
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	dp23result = -1;

	//����LPT����Ӵ�С����ӹ�ʱ��
	//sort(dp23job_array, dp23job_array + current_config.n, cmp);
	pmax23 = 0;
	for (int i = 0; i < current_config.n; i++)
	{
		if (pmax23 < dp23job_array[i].proc_time)
			pmax23 = dp23job_array[i].proc_time;
	}
	//�������鲢�ҳ�ʼ��
	new_dp23table();

	int m1 = 0, m2 = 0, x = 0;//��̨����
	//��ʼ����һ����
	m1 = 0;
	m2 = PSUM - m1;
	x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
	dp23table[0].x = x;
	dp23table[0].m1 = "";
	Q2[0] = 0;

	int job_num = 1;//��i������
	int q_length = 1;//�洢��һ�ε���ʱq���Ԫ�ظ���
	//��ʼ����
	while (job_num <= current_config.n)
	{
		int processtime = dp23job_array[job_num - 1].proc_time;//��ȡ��i������Ĵ���ʱ��
		int num = 0;//������ǰ������˶��ٴ�
		//ѭ��Q���ÿ��Ԫ�غ͵�i���������

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
					//�Ѽ�֮ǰ�����鶼���ƹ���  
					dp23table[a].m1 = dp23table[Q2[i]].m1;
					//�ѵ�ǰ�������
					dp23table[a].m1 += to_string(job_num) + " ";//��ǰ���������  �����Ŵ�1��ʼ
					Q2[q_length + num] = a;//q_length��0��ʼ
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
	for (int i = 0; i < current_config.D + pmax23 + 1; i++)
	{
		if (dp23table[i].x > dp23result)
		{
			dp23result = dp23table[i].x;
			bestresult = i;
		}
	}
	m1 = 0, m2 = 0, x = 0;
	//�������õĽ�����±�  ��������ȡ������һ�¼ӹ�����
	//cout << endl << "�����ڣ�" << current_config.D << endl;
	m1 = sqlit_sum(dp23table[bestresult].m1);
	m2 = PSUM - m1;
	x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
	QueryPerformanceCounter(&t2);
	dp23_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_dptable23();

	return x;
}
