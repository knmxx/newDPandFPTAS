#pragma once
#include<string>  
#include<vector>  
#include<cstdlib>

using namespace std;


//splitString�����Ĳ���˵����  
//����ֵ�Ƿָ���ַ��������С  
//strSrc ԭʼ�ַ���  
//strDelims �Զ���ķָ��ַ����ķָ�����  
//strDest �ָ����ַ������飬���ô���  


int  splitString(const string & strSrc, const std::string& strDelims, vector<string>& strDest)
{
	typedef std::string::size_type ST;
	string delims = strDelims;
	std::string STR;
	if (delims.empty()) delims = "/n/r";


	ST pos = 0, LEN = strSrc.size();
	while (pos < LEN) {
		STR = "";
		while ((delims.find(strSrc[pos]) != std::string::npos) && (pos < LEN)) ++pos;
		if (pos == LEN) return strDest.size();
		while ((delims.find(strSrc[pos]) == std::string::npos) && (pos < LEN)) STR += strSrc[pos++];
		//std::cout << "[" << STR << "]";  
		if (!STR.empty()) strDest.push_back(STR);
	}
	return strDest.size();
}

//���طָ��Ľ����
int sqlit_sum(string testStr)
{
	//string testStr = "1 2 3 4 5 6 7 8 9";
	string delims = ",- _ ����"; //����ָ����飬���Զ������ָ�������" ,./r"��  
	vector<string> splitStrs; //�ѷָ����ַ�������vector����  
	splitString(testStr, delims, splitStrs); //�����Զ���ķָ��  


	//��ʾ�ָ����ַ�������(��vector�洢)  
	int count = splitStrs.size();
	int sum = 0;
	//�������
	//cout << "m:  ";
	for (int i = 0; i < count; i++)
	{
		//cout << job_set[atoi(splitStrs[i].c_str()) - 1].proc_time << "  ";
		sum += job_set[atoi(splitStrs[i].c_str()) - 1].proc_time;
	}
	//cout << endl;
	//splitStrs.clear();
	//cout << sum << endl;
	return sum;
}

//���طָ��Ľ���� li��fptas  2̨����
int sqlit_sum_li_2(string testStr, Job job[10000],double a)
{
	//string testStr = "1 2 3 4 5 6 7 8 9";
	string delims = ",- _ ����"; //����ָ����飬���Զ������ָ�������" ,./r"��  
	vector<string> splitStrs; //�ѷָ����ַ�������vector����  
	splitString(testStr, delims, splitStrs); //�����Զ���ķָ��  


	//��ʾ�ָ����ַ�������(��vector�洢)  
	int count = splitStrs.size();
	int sum = 0;//Ҫ���ص�ԭ���������Ӧ�Ĵ���ʱ���
	double smallsum = 0;//С����Ĵ���ʱ���
	int num=0;//С����ĸ���
	
	//�������
	//cout << "m:  ";
	//cout << "��һ�������ļӹ�����" << endl;
	for (int i = 0; i < count; i++)
	{
		//cout << job_set[atoi(splitStrs[i].c_str()) - 1].proc_time << "  ";
		//�����ִ�����С����  С���񶼷ŵ�������ǲ���ǰ��Ķ��Ǵ����񣿣�
		//�ȰѴ����񶼼������
		if (job[atoi(splitStrs[i].c_str()) - 1].newp != 0)
		{
			//�����Ǵ�����
			sum += job[atoi(splitStrs[i].c_str()) - 1].newp;
			//cout << job[atoi(splitStrs[i].c_str()) - 1].proc_time << endl;
		}
		else
		{
			//������С����  ����һ��С����ĸ���  ���ڼ������ٵļӹ�ʱ��
			num++;
		}
	}
	//��С����
	//int quede_time=0;//ȱ��ʱ��
	//quede_time = num * ((int)(a*current_config.D));
	//cout << "С����" << endl;
	for (int i = 0; i < current_config.n; i++)
	{
		if (job_set[i].proc_time < a*current_config.D) 
		{
			//������С����
			//�ͼӽ�ȥ  ֱ��  ������num-1��ad
			if (smallsum < num*(a*current_config.D)) 
			{
				sum += job_set[i].proc_time;
				smallsum += job_set[i].proc_time;
				//cout << job_set[i].proc_time << endl;
			}
			else
				return sum;
				
		}
	}
	//cout << endl;
	//splitStrs.clear();
	//cout << sum << endl;
	return sum;
}

//���طָ��Ľ���� li��fptas  3̨��3̨���ϻ���
int sqlit_sum_li_n(string testStr, Job job[10000], double a)
{
	//string testStr = "1 2 3 4 5 6 7 8 9";
	string delims = ",- _ ����"; //����ָ����飬���Զ������ָ�������" ,./r"��  
	vector<string> splitStrs; //�ѷָ����ַ�������vector����  
	splitString(testStr, delims, splitStrs); //�����Զ���ķָ��  


	//��ʾ�ָ����ַ�������(��vector�洢)  
	int count = splitStrs.size();
	int sum = 0;//Ҫ���ص�ԭ���������Ӧ�Ĵ���ʱ���
	double smallsum = 0;//С����Ĵ���ʱ���
	int num = 0;//С����ĸ���

	//�������
	//cout << "m:  ";
	//cout << "��һ�������ļӹ�����" << endl;
	for (int i = 0; i < count; i++)
	{
		//cout << job_set[atoi(splitStrs[i].c_str()) - 1].proc_time << "  ";
		//�����ִ�����С����  С���񶼷ŵ�������ǲ���ǰ��Ķ��Ǵ����񣿣�
		//�ȰѴ����񶼼������
		if (job[atoi(splitStrs[i].c_str()) - 1].newp != 0)
		{
			//�����Ǵ�����
			sum += job[atoi(splitStrs[i].c_str()) - 1].newp;
			//cout << job[atoi(splitStrs[i].c_str()) - 1].proc_time << endl;
		}
		else
		{
			//������С����  ����һ��С����ĸ���  ���ڼ������ٵļӹ�ʱ��
			num++;
		}
	}
	//��С����
	//int quede_time=0;//ȱ��ʱ��
	//quede_time = num * ((int)(a*current_config.D));
	//cout << "С����" << endl;
	for (int i = 0; i < current_config.n; i++)
	{
		if (job_set[i].proc_time < a*current_config.D)
		{
			//������С����  ���Ҹ�����δ��ѡ��
			//�ͼӽ�ȥ  ֱ��  ������num-1��ad
			if (smallsum < num*(a*current_config.D) && yuanxulie_state[i] == 0) 
			{
				//�������Լ�����  �����������û�мӹ�
				sum += job_set[i].proc_time;
				smallsum += job_set[i].proc_time;
				yuanxulie_state[i] = 1;
				//cout << job_set[i].proc_time << endl;
			}
			else
				return sum;

		}
	}
	//cout << endl;
	//splitStrs.clear();
	//cout << sum << endl;
	return sum;
}