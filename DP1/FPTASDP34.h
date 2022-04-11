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
//��̨����
struct Con34
{
	double x;//����
	string m1;//�洢��һ̨�����ӹ�������
	string m2;//�洢�ڶ�̨�����ӹ�������
} con34;

double fptas34result = -1;
double fptas34_running_time = -1;
Job fptas34job_array[MAX_N];
Con34** fptas34table;//�⼯  ���ȣ�  d+Pmax+1*2
int** fptasLiQ34;//����
//Con24 bestresult;
int bestresult34_1;//��õĵ��ȷ������±�
int bestresult34_2;
//int *yuanxulie_state;//�洢ԭ�����Ƿ�ѡ��  ������þ�����ȷ���ʱ  ѡ��Ϊ1  δѡ��Ϊ0

int newd34 = -1;
double k34 = -1;
double e34 = 0.05;
double a34 = -1;
double psum34 = -1;
int newn34 = -1;//�洢�����еĹ�������
int newn34_small = -1;//��������С����ĸ���

int fptaspmax34 = 0;
double aad34 = -1;//(de r ta)�� * d  ÿ���������������������



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
	
	//memcpy(fptas24job_array, job_set, sizeof(job_set));//����
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	fptas34result = -1;

	//����LPT����Ӵ�С����ӹ�ʱ��
	//sort(dp23job_array, dp23job_array + current_config.n, cmp);
	a34 = e34 / 10;
	aad34 = a34 * a34 * current_config.D;
	//cout << a24 << endl;

	fptaspmax34 = 0;
	psum34 = 0;
	newn34 = 0;
	double psum34_small_job = 0;
	//����k 
	//k24 = (1.0 - a24) / (a24*a24);
	//��������Ĵ���ʱ���ȥa24*d����a24*a24*d�ı���  ��ôֱ�Ӱ������еĴ���ʱ���Ϊ����a24*a24*d�ļ������ͺ���  ����ʱ���������   
	//cout << "С�����СΪ��ad����" << a24 * current_config.D << endl;
	//cout << "ÿ������ʱ�䶼��aad����������" << aad24 << endl;
	for (int i = 0; i < current_config.n; i++)
	{
		if (job_set[i].proc_time < (a34*current_config.D))
		{
			//������С����  �ͼӺ�����
			psum34_small_job += job_set[i].proc_time;
		}
		else
		{
			newn34++;
			//������  ÿ������ʱ�䶼��aad��������
			fptas34job_array[newn34 - 1].proc_time = job_set[i].proc_time*1.0 / aad34;
			fptas34job_array[newn34 - 1].newp = job_set[i].proc_time;//����׷�ص�ԭ����  �洢��ԭ���е�ֵ
			psum34 += fptas34job_array[newn34 - 1].proc_time*aad34;
			if (fptaspmax34 < fptas34job_array[newn34 - 1].proc_time)
				fptaspmax34 = fptas34job_array[newn34 - 1].proc_time;
		}
	}
	//cout << "xiaorenwu=" << psum24_small_job<<endl;
	//����С����  ��һ��С�����ж��ٸ�
	if (psum34_small_job > 0)
	{
		//������С����
		if ((int)(psum34_small_job / (a34*current_config.D)) == 0)
			newn34_small = 1;
		else
			newn34_small = (int)(psum34_small_job / (a34*current_config.D)) + 1;//7/6=��������  ���Լ�һ
	}
	//newn24 += newn24_small;
	//��С����ӵ�������
	for (int i = 0; i < newn34_small; i++)
	{
		//�������񲻻���С���������  �Ͳ������ȥ��
		newn34++;
		fptas34job_array[newn34 - 1].proc_time = 1.0 / a34;//С������aad��1/a��  �Ǹ�����
		psum34 += fptas34job_array[newn34 - 1].proc_time*aad34;
		fptas34job_array[newn34 - 1].newp = 0;//�����ж��ǲ���С����
	}
	////��������µ�����
	//for (int i = 0; i < newn24; i++)
	//{
	//	cout << fptas24job_array[i].proc_time*aad24 <<endl;
	//}


	//cout << endl<<"k====="<<k24<<endl;
	//cout << endl << "������=====" << current_config.D << endl;
	//reset�ӹ�ʱ�估������
	/*cout << endl << "�������õĴ���ʱ��=====";*/
	//newd24 = psum24/ current_config.m;
	//newd24 = current_config.D;
	newd34 = current_config.D / aad34;
	//cout << endl << "psum=====" << psum23 << endl;
	//cout << endl << "newd24=====" << newd24 << endl;
	//�������鲢�ҳ�ʼ��
	new_fptas34table();

	
	double m1 = 0, m2 = 0, m3 = 0, x = 0;//��̨����
	//��ʼ����һ����
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
	int job_num = 1;//��i������
	int q_length = 1;//�洢��һ�ε���ʱq���Ԫ�ظ���
	//��ʼ����
	while (job_num <= newn34)
	{
		int processtime = fptas34job_array[job_num - 1].proc_time;//��ȡ��i������Ĵ���ʱ��
		int num = 0;//������ǰ������˶��ٴ�
		//ѭ��Q���ÿ��Ԫ�غ͵�i���������

		for (int i = 0; i < q_length; i++)
		{
			
			int a = 0, b = 0;
			//����i�������ڵ�һ�������ϼӹ�
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
					//��m1�ϼ�����
					fptas34table[a][b].x = x;
					//�Ѽ�֮ǰ�����鶼���ƹ���  
					fptas34table[a][b].m1 = fptas34table[fptasLiQ34[i][0]][fptasLiQ34[i][1]].m1;
					fptas34table[a][b].m2 = fptas34table[fptasLiQ34[i][0]][fptasLiQ34[i][1]].m2;
					//�ѵ�ǰ�������
					fptas34table[a][b].m1 += to_string(job_num) + " ";//��ǰ���������  �����Ŵ�1��ʼ
					fptasLiQ34[q_length + num][0] = a;//q_length��0��ʼ
					fptasLiQ34[q_length + num][1] = b;//q_length��0��ʼ
					num++;
					if (fptas34table[bestresult34_1][bestresult34_2].x < x)
					{
						bestresult34_1 = a;
						bestresult34_2 = b;
					}
				}
			}

			//����i�������ڵڶ��������ϼӹ�
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
					//��m2�ϼ�����
					fptas34table[a][b].x = x;
					//�Ѽ�֮ǰ�����鶼���ƹ���  
					fptas34table[a][b].m1 = fptas34table[fptasLiQ34[i][0]][fptasLiQ34[i][1]].m1;
					fptas34table[a][b].m2 = fptas34table[fptasLiQ34[i][0]][fptasLiQ34[i][1]].m2;
					//�ѵ�ǰ�������
					fptas34table[a][b].m2 += to_string(job_num) + " ";//��ǰ���������  �����Ŵ�1��ʼ
					fptasLiQ34[q_length + num][0] = a;//q_length��0��ʼ
					fptasLiQ34[q_length + num][1] = b;//q_length��0��ʼ
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
	//�������õĽ�����±�  ��������ȡ������һ�¼ӹ�����
	//cout << endl << "�����ڣ�" << current_config.D << endl;
	//cout << "fptas24table[bestresult].m1" << fptas24table[bestresult].m1 << endl;
	m1 = sqlit_sum_li_n(fptas34table[bestresult34_1][bestresult34_2].m1, fptas34job_array, a34);
	m2 = sqlit_sum_li_n(fptas34table[bestresult34_1][bestresult34_2].m2, fptas34job_array, a34);
	m3 = PSUM - m1 - m2;
	/*cout << "m1=" << m1 << endl;
	cout << "m2=" << m2 << endl;*/
	x = ((m1 > current_config.D) ? current_config.D : m1) 
		+ ((m2 > current_config.D) ? current_config.D : m2)
		+ ((m3 > current_config.D) ? current_config.D : m3);
	//cout << endl << "fptas�Ľ����" << fptas23result << "    xxx=" << xxx << endl;
	QueryPerformanceCounter(&t2);
	fptas34_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_fptastable34();

	return x;

}

