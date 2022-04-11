#pragma once
#include<string>  
#include<vector>  
#include<cstdlib>

using namespace std;


//splitString函数的参数说明：  
//返回值是分割后字符串数组大小  
//strSrc 原始字符串  
//strDelims 自定义的分割字符串的分割数组  
//strDest 分割后的字符串数组，引用传递  


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

//返回分割后的结果和
int sqlit_sum(string testStr)
{
	//string testStr = "1 2 3 4 5 6 7 8 9";
	string delims = ",- _ ，，"; //定义分割数组，可以定义多个分隔符，如" ,./r"等  
	vector<string> splitStrs; //把分割后的字符串存在vector里面  
	splitString(testStr, delims, splitStrs); //调用自定义的分割函数  


	//显示分割后的字符串数组(用vector存储)  
	int count = splitStrs.size();
	int sum = 0;
	//分配情况
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

//返回分割后的结果和 li的fptas  2台机器
int sqlit_sum_li_2(string testStr, Job job[10000],double a)
{
	//string testStr = "1 2 3 4 5 6 7 8 9";
	string delims = ",- _ ，，"; //定义分割数组，可以定义多个分隔符，如" ,./r"等  
	vector<string> splitStrs; //把分割后的字符串存在vector里面  
	splitString(testStr, delims, splitStrs); //调用自定义的分割函数  


	//显示分割后的字符串数组(用vector存储)  
	int count = splitStrs.size();
	int sum = 0;//要返回的原序列任务对应的处理时间和
	double smallsum = 0;//小任务的处理时间和
	int num=0;//小任务的个数
	
	//分配情况
	//cout << "m:  ";
	//cout << "第一个机器的加工任务：" << endl;
	for (int i = 0; i < count; i++)
	{
		//cout << job_set[atoi(splitStrs[i].c_str()) - 1].proc_time << "  ";
		//先区分大任务小任务  小任务都放到了最后是不是前面的都是大任务？？
		//先把大任务都计算出来
		if (job[atoi(splitStrs[i].c_str()) - 1].newp != 0)
		{
			//代表是大任务
			sum += job[atoi(splitStrs[i].c_str()) - 1].newp;
			//cout << job[atoi(splitStrs[i].c_str()) - 1].proc_time << endl;
		}
		else
		{
			//代表是小任务  计算一下小任务的个数  便于计算差多少的加工时间
			num++;
		}
	}
	//加小任务
	//int quede_time=0;//缺的时间
	//quede_time = num * ((int)(a*current_config.D));
	//cout << "小任务：" << endl;
	for (int i = 0; i < current_config.n; i++)
	{
		if (job_set[i].proc_time < a*current_config.D) 
		{
			//代表是小任务
			//就加进去  直到  超过（num-1）ad
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

//返回分割后的结果和 li的fptas  3台及3台以上机器
int sqlit_sum_li_n(string testStr, Job job[10000], double a)
{
	//string testStr = "1 2 3 4 5 6 7 8 9";
	string delims = ",- _ ，，"; //定义分割数组，可以定义多个分隔符，如" ,./r"等  
	vector<string> splitStrs; //把分割后的字符串存在vector里面  
	splitString(testStr, delims, splitStrs); //调用自定义的分割函数  


	//显示分割后的字符串数组(用vector存储)  
	int count = splitStrs.size();
	int sum = 0;//要返回的原序列任务对应的处理时间和
	double smallsum = 0;//小任务的处理时间和
	int num = 0;//小任务的个数

	//分配情况
	//cout << "m:  ";
	//cout << "第一个机器的加工任务：" << endl;
	for (int i = 0; i < count; i++)
	{
		//cout << job_set[atoi(splitStrs[i].c_str()) - 1].proc_time << "  ";
		//先区分大任务小任务  小任务都放到了最后是不是前面的都是大任务？？
		//先把大任务都计算出来
		if (job[atoi(splitStrs[i].c_str()) - 1].newp != 0)
		{
			//代表是大任务
			sum += job[atoi(splitStrs[i].c_str()) - 1].newp;
			//cout << job[atoi(splitStrs[i].c_str()) - 1].proc_time << endl;
		}
		else
		{
			//代表是小任务  计算一下小任务的个数  便于计算差多少的加工时间
			num++;
		}
	}
	//加小任务
	//int quede_time=0;//缺的时间
	//quede_time = num * ((int)(a*current_config.D));
	//cout << "小任务：" << endl;
	for (int i = 0; i < current_config.n; i++)
	{
		if (job_set[i].proc_time < a*current_config.D)
		{
			//代表是小任务  并且该任务未被选中
			//就加进去  直到  超过（num-1）ad
			if (smallsum < num*(a*current_config.D) && yuanxulie_state[i] == 0) 
			{
				//代表还可以加任务  并且这个任务还没有加过
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