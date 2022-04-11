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
struct Con33
{
	int x;//����
	string m1 = "";//�洢��һ̨�����ӹ�������
	string m2 = "";//�洢�ڶ�̨�����ӹ�������
} con33;

int fptas33result = -1;
double fptas33_running_time = -1;
Job fptas33job_array[MAX_N];
Con33** fptas33table;//�⼯  ���ȣ�  d+Pmax+1*2
int** fptasQ3;//����

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
	memcpy(fptas33job_array, job_set, sizeof(job_set));//����
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	fptas33result = -1;

	//����LPT����Ӵ�С����ӹ�ʱ��
	//sort(dp33job_array, dp33job_array + current_config.n, cmp);
	fptaspmax33 = 0;
	psum33 = 0;
	//����k 
	k33 = (e33*current_config.D) / ((current_config.m - 1)*current_config.n);
	//cout << endl << "k=====" << k33 << endl;
	if (k33 <= 1)
	{
		k33 = 1.0;
	}
	
	//reset�ӹ�ʱ�估������
	/*cout << endl << "�������õĴ���ʱ��=====";*/
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
	//�������鲢�ҳ�ʼ��
	new_fptas33table();

	int m1 = 0, m2 = 0, m3 = 0, x = 0;//��̨����
	//��ʼ����һ����
	m1 = 0;
	m2 = 0;
	m3 = psum33 - m1 - m2;
	x = ((m1 > newd33) ? newd33 : m1) + ((m2 > newd33) ? newd33 : m2) + ((m3 > newd33) ? newd33 : m3);
	fptas33table[0][0].x = x;
	fptas33table[0][0].m1 = "";
	fptas33table[0][0].m2 = "";
	fptasQ3[0][0] = m1;
	fptasQ3[0][1] = m2;

	int job_num = 1;//��i������
	int q_length = 1;//�洢��һ�ε���ʱq���Ԫ�ظ���
	//��ʼ����
	while (job_num <= current_config.n)
	{
		int processtime = fptas33job_array[job_num - 1].proc_time;//��ȡ��i������Ĵ���ʱ��
		int num = 0;//������ǰ������˶��ٴ�
		//ѭ��Q���ÿ��Ԫ�غ͵�i���������

		for (int i = 0; i < q_length; i++)
		{
			int a = 0, b = 0;
			//����i�������ڵ�һ�������ϼӹ�
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
					//��m1�ϼ�����
					fptas33table[a][b].x = x;
					//�Ѽ�֮ǰ�����鶼���ƹ���  ����m1��m2��
					fptas33table[a][b].m1 = fptas33table[fptasQ3[i][0]][fptasQ3[i][1]].m1;
					fptas33table[a][b].m2 = fptas33table[fptasQ3[i][0]][fptasQ3[i][1]].m2;
					//�ѵ�ǰ����ӵ�m1��
					fptas33table[a][b].m1 += to_string(job_num)+" ";//��ǰ���������  �����Ŵ�1��ʼ
					fptasQ3[q_length + num][0] = a;//q_length��0��ʼ
					fptasQ3[q_length + num][1] = b;//q_length��0��ʼ
					num++;
				}
			}
			//����i�������ڵڶ��������ϼӹ�
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
					//��m2�ϼ�����
					fptas33table[a][b].x = x;
					//�Ѽ�֮ǰ�����鶼���ƹ��� ����m1,m2��
					fptas33table[a][b].m1 = fptas33table[fptasQ3[i][0]][fptasQ3[i][1]].m1;
					fptas33table[a][b].m2 = fptas33table[fptasQ3[i][0]][fptasQ3[i][1]].m2;
					//�ѵ�ǰ����ӵ�m2��
					fptas33table[a][b].m2 += to_string(job_num) +" ";//��ǰ���������  �����Ŵ�1��ʼ
					fptasQ3[q_length + num][0] = a;//q_length��0��ʼ
					fptasQ3[q_length + num][1] = b;//q_length��0��ʼ
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
	int bestresult_1 = 0, bestresult_2 = 0;//��õĽ�����±�  ��ά����  ���±�
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
	//�������õĽ�����±�  ��������ȡ������һ�¼ӹ�����
	//cout << endl << "�����ڣ�" << current_config.D << endl;
	//cout << fptas33table[bestresult_1][bestresult_2].m1  ;
	m1 = sqlit_sum(fptas33table[bestresult_1][bestresult_2].m1);
	m2 = sqlit_sum(fptas33table[bestresult_1][bestresult_2].m2);
	m3 = PSUM - m1 - m2;
	x = ((m1 > current_config.D) ? current_config.D : m1) +
		((m2 > current_config.D) ? current_config.D : m2) +
		((m3 > current_config.D) ? current_config.D : m3);
	//cout << endl << "fptas�Ľ����" << fptas33result << "    xxx=" << xxx << endl;
	QueryPerformanceCounter(&t2);
	fptas33_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_fptastable33();

	return x;
}
