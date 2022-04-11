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
//����汾���������Ƕ��ٱ�  ��FPTASDP242
struct Con24
{
	double x=0;//����
	string m1;//M1�ϵļӹ�����
} con24;

double fptas24result = -1;
double fptas24_running_time = -1;
Job fptas24job_array[MAX_N];
Con24* fptas24table;//�⼯  ���ȣ�  d+Pmax+1*2
int* fptasLiQ24;//����
//Con24 bestresult;
int bestresult24;//��õĵ��ȷ������±�

int newd24 = -1;
double k24 = -1;
double e24 = 0.075;
double a24 = -1;
double psum24 = -1;
int newn24 = -1;//�洢�����еĹ�������
int newn24_small = -1;//��������С����ĸ���

int fptaspmax24 = 0;
double aad24 = -1;//(de r ta)�� * d  ÿ���������������������



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
	//memcpy(fptas24job_array, job_set, sizeof(job_set));//����
	LARGE_INTEGER cpu_freq, t1, t2;
	QueryPerformanceFrequency(&cpu_freq);
	QueryPerformanceCounter(&t1);

	fptas24result = -1;

	//����LPT����Ӵ�С����ӹ�ʱ��
	//sort(dp23job_array, dp23job_array + current_config.n, cmp);
	a24 = e24 / 10;
	aad24 = a24 * a24 * current_config.D;
	//cout << a24 << endl;
	
	fptaspmax24 = 0;
	psum24 = 0;
	newn24 = 0;
	double psum24_small_job = 0;
	//����k 
	//k24 = (1.0 - a24) / (a24*a24);
	//��������Ĵ���ʱ���ȥa24*d����a24*a24*d�ı���  ��ôֱ�Ӱ������еĴ���ʱ���Ϊ����a24*a24*d�ļ������ͺ���  ����ʱ���������   
	//cout << "С�����СΪ��ad����" << a24 * current_config.D << endl;
	//cout << "ÿ������ʱ�䶼��aad����������" << aad24 << endl;
	for (int i = 0; i < current_config.n; i++)
	{
		if (job_set[i].proc_time < (a24*current_config.D))
		{
			//������С����  �ͼӺ�����
			psum24_small_job += job_set[i].proc_time;
		}
		else
		{
			newn24++;
			//������  ÿ������ʱ�䶼��aad��������  
			fptas24job_array[newn24 - 1].proc_time = job_set[i].proc_time*1.0 / aad24;
			fptas24job_array[newn24 - 1].newp = job_set[i].proc_time;//����׷�ص�ԭ����  �洢��ԭ���е�ֵ
			psum24 += fptas24job_array[newn24 - 1].proc_time*aad24;
			if (fptaspmax24 < fptas24job_array[newn24 - 1].proc_time)
				fptaspmax24 = fptas24job_array[newn24 - 1].proc_time;
		}
	}
	//cout << "xiaorenwu=" << psum24_small_job<<endl;
	//����С����  ��һ��С�����ж��ٸ�
	if (psum24_small_job > 0)
	{
		//������С����
		if ((int)(psum24_small_job / (a24*current_config.D)) == 0)
			newn24_small = 1;
		else
			newn24_small = (int)(psum24_small_job / (a24*current_config.D)) + 1;//7/6=��������  ���Լ�һ
	}
	//newn24 += newn24_small;
	//��С����ӵ�������
	for (int i = 0; i < newn24_small; i++)
	{
		//�������񲻻���С���������  �Ͳ������ȥ�� ***�������С����  ��ô�����ʱ�����С�����С
		newn24++;
		fptas24job_array[newn24 - 1].proc_time = 1.0 / a24;//С������aad��1/a��  �Ǹ�����
		psum24 += fptas24job_array[newn24 - 1].proc_time*aad24;
		fptas24job_array[newn24 - 1].newp = 0;//�����ж��ǲ���С����
	}
	if (fptaspmax24 < (int)(1.0 / a24))
		fptaspmax24 = (int)(1.0 / a24);
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
	newd24 = current_config.D / aad24;
	//cout << endl << "psum=====" << psum23 << endl;
	//cout << endl << "newd24=====" << newd24 << endl;
	//�������鲢�ҳ�ʼ��
	new_fptas24table();


	double m1 = 0, m2 = 0, x = 0;//��̨����
	//��ʼ����һ����
	m1 = 0;
	m2 = psum24 - m1;
	x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
	fptas24table[0].x = x;
	fptas24table[0].m1 = "";
	fptasLiQ24[0] = 0;
	bestresult24 = 0;
	int job_num = 1;//��i������
	int q_length = 1;//�洢��һ�ε���ʱq���Ԫ�ظ���
	//��ʼ����
	while (job_num <= newn24)
	{
		int processtime = fptas24job_array[job_num - 1].proc_time;//��ȡ��i������Ĵ���ʱ��
		int num = 0;//������ǰ������˶��ٴ�
		//ѭ��Q���ÿ��Ԫ�غ͵�i���������

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
					//�Ѽ�֮ǰ�����鶼���ƹ���  
					fptas24table[a].m1 = fptas24table[fptasLiQ24[i]].m1;
					//�ѵ�ǰ�������
					fptas24table[a].m1 += to_string(job_num) + " ";//��ǰ���������  �����Ŵ�1��ʼ
					fptasLiQ24[q_length + num] = a;//q_length��0��ʼ
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
	//�������õĽ�����±�  ��������ȡ������һ�¼ӹ�����
	//cout << endl << "�����ڣ�" << current_config.D << endl;
	//cout << "fptas24table[bestresult].m1" << fptas24table[bestresult].m1 << endl;
	m1 = sqlit_sum_li_2(fptas24table[bestresult24].m1, fptas24job_array, a24);
	m2 = PSUM - m1;
	/*cout << "m1=" << m1 << endl;
	cout << "m2=" << m2 << endl;*/
	x = ((m1 > current_config.D) ? current_config.D : m1) + ((m2 > current_config.D) ? current_config.D : m2);
	//cout << endl << "fptas�Ľ����" << fptas23result << "    xxx=" << xxx << endl;
	QueryPerformanceCounter(&t2);
	fptas24_running_time = 1.0 * (t2.QuadPart - t1.QuadPart) / cpu_freq.QuadPart;
	free_fptastable24();

	return x;

}

