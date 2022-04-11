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
struct Con43
{
	int x;//����
	string m1 = "";//�洢��һ̨�����ӹ�������
	string m2 = "";//�洢�ڶ�̨�����ӹ�������
	string m3 = "";//�洢����̨�����ӹ�������
} con43;

int fptas43result = -1;
double fptas43_running_time = -1;
Job fptas43job_array[MAX_N];
Con43*** fptas43table;//�⼯  ���ȣ�  d+Pmax+1*2
int** fptasQ4;//����

int newd43 = -1;
double k43 = -1;
double e43 = 0.05;
int psum43 = -1;

int fptaspmax43 = 0;

int num43 = 0;//������ǰ������˶��ٴ�
int q_length43 = 1;//�洢��һ�ε���ʱq���Ԫ�ظ���
int job_num43 = 1;//��i������
//
//��һ��  ����ѭ���Ĵ�����Ϊһ��ֵ
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

//�µ������±꣺a��b��c   �ӵ������±�i��j��k  �ӵ���m̨������  m��1��ʼ 
void dispatch43(int a, int b, int c,int i,int j,int k,int m)//�������   
{
	int m1 = 0, m2 = 0, m3 = 0, m4 = 0, x = 0;//��̨����11
	if (a < (newd43 + fptaspmax43) && b < (newd43 + fptaspmax43) && c < (newd43 + fptaspmax43))
	{
		m1 = a;
		m2 = b;
		m3 = c;
		m4 = psum43 - a - b - c;
		//cout << "���أ�" << m1 << "  " << m2 << "  " << m3 << "  " << m4 << endl;
		x = ((m1 > newd43) ? newd43 : m1) +
			((m2 > newd43) ? newd43 : m2) +
			((m3 > newd43) ? newd43 : m3) +
			((m4 > newd43) ? newd43 : m4);
		if (fptas43table[a][b][c].x == -1 && fptas43table[a][c][b].x == -1 && 
			fptas43table[b][a][c].x == -1 && fptas43table[b][c][a].x == -1 && 
			fptas43table[c][a][b].x == -1 && fptas43table[c][b][a].x == -1)
		{
			fptas43table[a][b][c].x = x;
			//�Ѽ�֮ǰ�����鶼���ƹ���  ����m1��m2��
			fptas43table[a][b][c].m1 =
				fptas43table[i][j][k].m1;
			fptas43table[a][b][c].m2 =
				fptas43table[i][j][k].m2;
			fptas43table[a][b][c].m3 =
				fptas43table[i][j][k].m3;
			switch (m) {
			case 1:
				//�ѵ�ǰ����ӵ�m1��
				fptas43table[a][b][c].m1 += to_string(job_num43) + " ";//��ǰ���������  �����Ŵ�1��ʼ
				break;
			case 2:
				//�ѵ�ǰ����ӵ�m2��
				fptas43table[a][b][c].m2 += to_string(job_num43) + " ";//��ǰ���������  �����Ŵ�1��ʼ
				break;
			case 3:
				//�ѵ�ǰ����ӵ�m3��
				fptas43table[a][b][c].m3 += to_string(job_num43) + " ";//��ǰ���������  �����Ŵ�1��ʼ
				break;
			}
			fptasQ4[q_length43 + num43][0] = a;//q_length��0��ʼ
			fptasQ4[q_length43 + num43][1] = b;//q_length��0��ʼ
			fptasQ4[q_length43 + num43][2] = c;//q_length��0��ʼ
			num43++;
		}
	}
}

int fptas43()
{
	memcpy(fptas43job_array, job_set, sizeof(job_set));//����
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	fptas43result = -1;

	//����LPT����Ӵ�С����ӹ�ʱ��
	//sort(dp43job_array, dp43job_array + current_config.n, cmp);
	fptaspmax43 = 0;
	psum43 = 0;
	//����k 
	k43 = (e43*current_config.D) / ((current_config.m - 1)*current_config.n);
	if (k43 <= 1)
	{
		k43 = 1.0;
	}
	//cout << endl<<"k====="<<k43<<endl;
	//reset�ӹ�ʱ�估������
	/*cout << endl << "�������õĴ���ʱ��=====";*/
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
	//�������鲢�ҳ�ʼ��
	new_fptas43table();

	


	int m1 = 0, m2 = 0, m3 = 0, m4 = 0, x = 0;//��̨����
	//��ʼ����һ����
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
	job_num43 = 1;//��i������
	//��ʼ����
	while (job_num43 <= current_config.n)
	{
		int processtime = fptas43job_array[job_num43 - 1].proc_time;//��ȡ��i������Ĵ���ʱ��
		num43 = 0;//������ǰ������˶��ٴ�
		//ѭ��Q���ÿ��Ԫ�غ͵�i���������

		for (int i = 0; i < q_length43; i++)
		{
			int a = 0, b = 0, c = 0;
			//����i�������ڵ�һ�������ϼӹ�
			a = processtime + fptasQ4[i][0];
			b = fptasQ4[i][1];
			c = fptasQ4[i][2];
			dispatch43(a, b, c, fptasQ4[i][0], fptasQ4[i][1], fptasQ4[i][2], 1);
			//����i�������ڵڶ��������ϼӹ�
			a = fptasQ4[i][0];
			b = processtime + fptasQ4[i][1];
			c = fptasQ4[i][2];
			dispatch43(a, b, c, fptasQ4[i][0], fptasQ4[i][1], fptasQ4[i][2], 2);
			//����i�������ڵ����������ϼӹ�
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
	int bestresult_1 = 0, bestresult_2 = 0, bestresult_3 = 0;//��õĽ�����±�  ��ά����  ���±�
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
	//�������õĽ�����±�  ��������ȡ������һ�¼ӹ�����
	//cout << endl << "�����ڣ�" << current_config.D << endl;
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
	//cout << endl << "fptas�Ľ����" << fptas33result << "    xxx=" << xxx << endl;
	QueryPerformanceCounter(&t2);
	fptas43_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_fptastable43();

	return x;
}
