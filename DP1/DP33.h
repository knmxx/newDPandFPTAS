#pragma once

#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "job.h"
#include "tool.h"
using namespace std;
//ȥ��ѭ������������ȷ���

//��̨����
struct ConDP33
{
	int x;//����
	string m1 = "";//�洢��һ̨�����ӹ�������
	string m2 = "";//�洢�ڶ�̨�����ӹ�������
} condp33;

int dp33result = -1;
double dp33_running_time = -1;
Job dp33job_array[MAX_N];
ConDP33** dp33table;//�⼯  ���ȣ�  d+Pmax+1*2
int** Q3;//����

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
	memcpy(dp33job_array, job_set, sizeof(job_set));//����
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	dp33result = -1;

	//����LPT����Ӵ�С����ӹ�ʱ��
	//sort(dp33job_array, dp33job_array + current_config.n, cmp);
	pmax33 = 0;
	for (int i = 0; i < current_config.n; i++)
	{
		if (pmax33 < dp33job_array[i].proc_time)
			pmax33 = dp33job_array[i].proc_time;
	}
	//�������鲢�ҳ�ʼ��
	new_dp33table();

	int m1 = 0, m2 = 0, m3 = 0, x = 0;//��̨����
	//��ʼ����һ����
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
	int job_num = 1;//��i������
	int q_length = 1;//�洢��һ�ε���ʱq���Ԫ�ظ���
	//��ʼ����
	while (job_num <= current_config.n)
	{
		int processtime = dp33job_array[job_num - 1].proc_time;//��ȡ��i������Ĵ���ʱ��
		int num = 0;//������ǰ������˶��ٴ�
		//ѭ��Q���ÿ��Ԫ�غ͵�i���������


		for (int i = 0; i < q_length; i++)
		{
			int a = 0, b = 0;
			//����i�������ڵ�һ�������ϼӹ�
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
					//�Ѽ�֮ǰ�����鶼���ƹ���  ����m1��m2��
					dp33table[a][b].m1 = dp33table[Q3[i][0]][Q3[i][1]].m1;
					dp33table[a][b].m2 = dp33table[Q3[i][0]][Q3[i][1]].m2;
					//�ѵ�ǰ����ӵ�m1��
					dp33table[a][b].m1 += to_string(job_num) + " ";//��ǰ���������  �����Ŵ�1��ʼ
					Q3[q_length + num][0] = a;//q_length��0��ʼ
					Q3[q_length + num][1] = b;//q_length��0��ʼ
					num++;
				}
			}
			//����i�������ڵڶ��������ϼӹ�
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
					//�Ѽ�֮ǰ�����鶼���ƹ���  ����m1��m2��
					dp33table[a][b].m1 = dp33table[Q3[i][0]][Q3[i][1]].m1;
					dp33table[a][b].m2 = dp33table[Q3[i][0]][Q3[i][1]].m2;
					//�ѵ�ǰ����ӵ�m2��
					dp33table[a][b].m2 += to_string(job_num) + " ";//��ǰ���������  �����Ŵ�1��ʼ
					Q3[q_length + num][0] = a;//q_length��0��ʼ
					Q3[q_length + num][1] = b;//q_length��0��ʼ
					num++;
				}

				//���ٸĽ�һ��   �Ľ��ܳɹ�  ���н����һ��  ����ʱ��ûʲô̫����
				//if (dp33table[a][b] == -1&& dp33table[b][a] == -1)
				//{
				//	dp33table[a][b] = x;
				//	Q3[q_length + num][0] = a;//q_length��0��ʼ
				//	Q3[q_length + num][1] = b;//q_length��0��ʼ
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
	int bestresult_1 = 0, bestresult_2 = 0;//��õĽ�����±�  ��ά����  ���±�
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
	//�������õĽ�����±�  ��������ȡ������һ�¼ӹ�����
	//cout << endl << "�����ڣ�" << current_config.D << endl;
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
