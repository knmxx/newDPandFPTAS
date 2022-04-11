#pragma once
#include <algorithm>
#include <vector>
using namespace std;

const int MAX_M = 50;
const int MAX_N = 10000;
const int maxnumber = 9;
const double eps = 1e-6;//一个极小的数
int *yuanxulie_state;//存储原序列是否被选中  在最后获得具体调度方案时  选中为1  未选中为0
/*
Job:
proc_time i: processing time when the job being proceed on machine i


test_data is a global variable, stores the current solving problem
*/

struct Job
{
	int  proc_time;
	int due_date;
	double newp;
	double newd;
}job_set[MAX_N];
int PSUM = 0;
struct Configure
{
	int m, n;
	int caseindex;
	int D;
} current_config;



int job_late_work(Job& job, int completion_time_finished_this_job)
{
	return min(job.proc_time, max(0, completion_time_finished_this_job - current_config.D));
}

bool job_cmp_reverse_proc_time(const Job& left, const Job& right) {
	return left.proc_time > right.proc_time;
}

bool order_job_cmp_reverse_proc_time(int left, int right) {
	return job_cmp_reverse_proc_time(job_set[left], job_set[right]);
}

bool cmp(Job a, Job b)//从大到小排序
{
	return a.proc_time > b.proc_time;
}