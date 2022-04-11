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
//ȥ��dispath����ֱ��д

struct ConDP43
{
	int x;//����
	string m1 = "";//�洢��һ̨�����ӹ�������
	string m2 = "";//�洢�ڶ�̨�����ӹ�������
	string m3 = "";//�洢����̨�����ӹ�������
} condp43;

int dp43result = -1;
double dp43_running_time = -1;
Job dp43job_array[MAX_N];
ConDP43*** dp43table;//�⼯  ���ȣ�  d+Pmax+1*2
int** Q4;//����

int pmax43 = 0;


void new_dp43table()
{
	dp43table = new ConDP43**[current_config.D + pmax43 + 1];
	Q4 = new int*[(current_config.D + pmax43 + 1)*
		(current_config.D + pmax43 + 1)*
		(current_config.D + pmax43 + 1)];
	for (int i = 0; i < (current_config.D + pmax43 + 1); i++)
	{
		dp43table[i] = new ConDP43*[current_config.D + pmax43 + 1];
	}
	for (int i = 0; i < (current_config.D + pmax43 + 1); i++)
		for (int j = 0; j < (current_config.D + pmax43 + 1); j++)
		{
			dp43table[i][j] = new ConDP43[current_config.D + pmax43 + 1];
		}
	for (int i = 0; i < (current_config.D + pmax43 + 1)*(current_config.D + pmax43 + 1)*(current_config.D + pmax43 + 1); i++)
	{
		Q4[i] = new int[3];
	}
	for (int i = 0; i < (current_config.D + pmax43 + 1); i++)
		for (int j = 0; j < (current_config.D + pmax43 + 1); j++)
			for (int k = 0; k < (current_config.D + pmax43 + 1); k++)
			{
				dp43table[i][j][k].x = -1;
			}
}

void free_dptable43()
{
	for (int i = 0; i < (current_config.D + pmax43 + 1); i++)
	{
		for (int j = 0; j < (current_config.D + pmax43 + 1); j++)
		{
			delete[] dp43table[i][j];
		}
		delete[] dp43table[i];
	}
	for (int i = 0; i < (current_config.D + pmax43 + 1)*(current_config.D + pmax43 + 1)*(current_config.D + pmax43 + 1); i++)
	{
		delete[] Q4[i];
	}
	delete[] dp43table;
	delete[] Q4;
}



int dp43()
{
	memcpy(dp43job_array, job_set, sizeof(job_set));//����
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	dp43result = -1;

	//����LPT����Ӵ�С����ӹ�ʱ��
	//sort(dp43job_array, dp43job_array + current_config.n, cmp);
	pmax43 = 0;
	for (int i = 0; i < current_config.n; i++)
	{
		if (pmax43 < dp43job_array[i].proc_time)
			pmax43 = dp43job_array[i].proc_time;
	}
	//�������鲢�ҳ�ʼ��
	new_dp43table();

	int m1 = 0, m2 = 0, m3 = 0, m4 = 0, x = 0;//��̨����
	//��ʼ����һ����
	m1 = 0;
	m2 = 0;
	m3 = 0;
	m4 = PSUM - m1 - m2 - m3;
	x = ((m1 > current_config.D) ? current_config.D : m1)
		+ ((m2 > current_config.D) ? current_config.D : m2)
		+ ((m3 > current_config.D) ? current_config.D : m3)
		+ ((m4 > current_config.D) ? current_config.D : m4);
	dp43table[0][0][0].x = x;
	dp43table[0][0][0].m1 = "";
	dp43table[0][0][0].m2 = "";
	dp43table[0][0][0].m3 = "";
	Q4[0][0] = m1;
	Q4[0][1] = m2;
	Q4[0][2] = m3;
	int job_num = 1;//��i������
	int q_length = 1;//�洢��һ�ε���ʱq���Ԫ�ظ���
	//��ʼ����
	while (job_num <= current_config.n)
	{
		int processtime = dp43job_array[job_num - 1].proc_time;//��ȡ��i������Ĵ���ʱ��
		int num = 0;//������ǰ������˶��ٴ�
		//ѭ��Q���ÿ��Ԫ�غ͵�i���������


		for (int i = 0; i < q_length; i++)
		{
			int a = 0, b = 0, c = 0;
			//����i�������ڵ�һ�������ϼӹ�
			a = processtime + Q4[i][0];
			b = Q4[i][1];
			c = Q4[i][2];
			if (a < (current_config.D + pmax43) && b < (current_config.D + pmax43) && c < (current_config.D + pmax43))
			{
				m1 = a;
				m2 = b;
				m3 = c;
				m4 = PSUM - a - b - c;
				x = ((m1 > current_config.D) ? current_config.D : m1)
					+ ((m2 > current_config.D) ? current_config.D : m2)
					+ ((m3 > current_config.D) ? current_config.D : m3)
					+ ((m4 > current_config.D) ? current_config.D : m4);
				if (dp43table[a][b][c].x == -1)
				{
					dp43table[a][b][c].x = x;
					//�Ѽ�֮ǰ�����鶼���ƹ���  ����m1��m2��
					dp43table[a][b][c].m1 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m1 + 
						to_string(job_num) + " ";
					dp43table[a][b][c].m2 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m2;
					dp43table[a][b][c].m3 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m3;
					//�ѵ�ǰ����ӵ�m1��
					//dp43table[a][b][c].m1 += to_string(job_num) + " ";//��ǰ���������  �����Ŵ�1��ʼ
					Q4[q_length + num][0] = a;//q_length��0��ʼ
					Q4[q_length + num][1] = b;//q_length��0��ʼ
					Q4[q_length + num][2] = c;//q_length��0��ʼ
					num++;
				}
			}
			//����i�������ڵڶ��������ϼӹ�
			a = Q4[i][0];
			b = processtime + Q4[i][1];
			c = Q4[i][2];
			if (a < (current_config.D + pmax43) && b < (current_config.D + pmax43) && c < (current_config.D + pmax43))
			{
				m1 = a;
				m2 = b;
				m3 = c;
				m4 = PSUM - a - b - c;
				x = ((m1 > current_config.D) ? current_config.D : m1)
					+ ((m2 > current_config.D) ? current_config.D : m2)
					+ ((m3 > current_config.D) ? current_config.D : m3)
					+ ((m4 > current_config.D) ? current_config.D : m4);
				if (dp43table[a][b][c].x == -1)
				{
					dp43table[a][b][c].x = x;
					//�Ѽ�֮ǰ�����鶼���ƹ���  ����m1��m2��
					dp43table[a][b][c].m1 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m1;
					dp43table[a][b][c].m2 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m2 + to_string(job_num) + " ";
					dp43table[a][b][c].m3 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m3;
					//�ѵ�ǰ����ӵ�m2��
					//dp43table[a][b][c].m2 += to_string(job_num) + " ";//��ǰ���������  �����Ŵ�1��ʼ
					Q4[q_length + num][0] = a;//q_length��0��ʼ
					Q4[q_length + num][1] = b;//q_length��0��ʼ
					Q4[q_length + num][2] = c;//q_length��0��ʼ
					num++;
				}
			}
			//����i�������ڵڶ��������ϼӹ�
			a = Q4[i][0];
			b = Q4[i][1];
			c = processtime + Q4[i][2];
			if (a < (current_config.D + pmax43) && b < (current_config.D + pmax43) && c < (current_config.D + pmax43))
			{
				m1 = a;
				m2 = b;
				m3 = c;
				m4 = PSUM - a - b - c;
				x = ((m1 > current_config.D) ? current_config.D : m1)
					+ ((m2 > current_config.D) ? current_config.D : m2)
					+ ((m3 > current_config.D) ? current_config.D : m3)
					+ ((m4 > current_config.D) ? current_config.D : m4);
				if (dp43table[a][b][c].x == -1)
				{
					dp43table[a][b][c].x = x;
					//�Ѽ�֮ǰ�����鶼���ƹ���  ����m1��m2��
					dp43table[a][b][c].m1 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m1;
					dp43table[a][b][c].m2 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m2;
					dp43table[a][b][c].m3 = dp43table[Q4[i][0]][Q4[i][1]][Q4[i][2]].m3 + to_string(job_num) + " ";
					//�ѵ�ǰ����ӵ�m3��
					//dp43table[a][b][c].m3 += to_string(job_num) + " ";//��ǰ���������  �����Ŵ�1��ʼ
					Q4[q_length + num][0] = a;//q_length��0��ʼ
					Q4[q_length + num][1] = b;//q_length��0��ʼ
					Q4[q_length + num][2] = c;//q_length��0��ʼ
					num++;
				}
			}
		}

		/*for (int i = 0; i < 100; i++)
			cout << Q3[i][0] << "  ";
		cout << endl;*/
		q_length += num;
		//cout << "length------------" << q_length << endl;
		job_num++;
	}
	int bestresult_1 = 0, bestresult_2 = 0, bestresult_3 = 0;//��õĽ�����±�  ��ά����  ���±�
	for (int i = 0; i < current_config.D + pmax43 + 1; i++)
	{
		for (int j = 0; j < current_config.D + pmax43 + 1; j++)
		{
			for (int k = 0; k < current_config.D + pmax43 + 1; k++)
			{
				if (dp43table[i][j][k].x > dp43result)
				{
					dp43result = dp43table[i][j][k].x;
					bestresult_1 = i;
					bestresult_2 = j;
					bestresult_3 = k;
				}
			}
		}
	}

	m1 = 0, m2 = 0, m3 = 0, m4 = 0, x = 0;
	//�������õĽ�����±�  ��������ȡ������һ�¼ӹ�����
	//cout << endl << "�����ڣ�" << current_config.D << endl;
	//cout << fptas33table[bestresult_1][bestresult_2].m1  ;
	//cout << dp43table[bestresult_1][bestresult_2][bestresult_3].m1 << endl;
	//cout << dp43table[bestresult_1][bestresult_2][bestresult_3].m2 << endl;
	//cout << dp43table[bestresult_1][bestresult_2][bestresult_3].m3 << endl;
	m1 = sqlit_sum(dp43table[bestresult_1][bestresult_2][bestresult_3].m1);
	m2 = sqlit_sum(dp43table[bestresult_1][bestresult_2][bestresult_3].m2);
	m3 = sqlit_sum(dp43table[bestresult_1][bestresult_2][bestresult_3].m3);
	m4 = PSUM - m1 - m2 - m3;
	x = ((m1 > current_config.D) ? current_config.D : m1) +
		((m2 > current_config.D) ? current_config.D : m2) +
		((m3 > current_config.D) ? current_config.D : m3) +
		((m4 > current_config.D) ? current_config.D : m4);

	QueryPerformanceCounter(&t2);
	dp43_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_dptable43();

	return x;
}
