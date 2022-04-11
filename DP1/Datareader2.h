#pragma once


#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Job.h"


using namespace std;

class DataReader2
{
public:
	DataReader2(const char* filename);
	virtual ~DataReader2();
	vector<Job> next_case();
	bool has_next();
	static bool to_array(Job* jobArray, int max_size, vector<Job> jobs);
	Configure get_current_configure();
	int get_current_case();

	vector<vector<Job> > data;

protected:

private:
	int current_case;
	vector<Configure> configs;
};

DataReader2::DataReader2(const char* filename)
{
	//ctor

	data.clear();
	configs.clear();
	FILE *fp;
	errno_t err = fopen_s(&fp, filename, "r");
	if (err != 0)
	{
		printf("[DATAREADER]Cannot open %s\n", filename);
		return;
	}
	int c, n, m;
	while (!feof(fp))
	{
		if (fscanf_s(fp, "P = (cases = %d, n = %d, m = %d)\n", &c, &n, &m) != 3)
			break;
		for (int c_i = 0; c_i < c; c_i++)
		{
			vector<Job> job_data;
			fscanf_s(fp, "[p, d] = ");
			for (int id = 0; id < n; id++)
			{
				Job job;
				fscanf_s(fp, "[%d, %d] ", &job.proc_time, &job.due_date);
				job_data.push_back(job);

			}
			data.push_back(job_data);
			fscanf_s(fp, "\n");

			Configure config;
			config.m = m;
			config.n = n;
			configs.push_back(config);
		}
	}
	current_case = 0;
	printf("%d case(s) loaded\n", data.size());
}

DataReader2::~DataReader2()
{
	//dtor
}

vector<Job> DataReader2::next_case()
{
	if (current_case < data.size())
	{
		vector<Job> current_jobs = data[current_case];
		DataReader2::to_array(job_set, MAX_N, current_jobs);

		Configure& config = configs[current_case];
		current_config.m = config.m;
		current_config.n = config.n;
		current_config.caseindex = current_case;
		current_case++;
		return current_jobs;
	}

	return vector<Job>();
}

bool DataReader2::has_next()
{
	return current_case < data.size();
}

bool DataReader2::to_array(Job* jobArray, int max_size, vector<Job> jobs)
{
	if (jobs.size() > max_size)
	{
		printf("[DATARAEDER] n = %d is too large\n", jobs.size());
		return false;
	}
	int t = 0;
	for (int i = 0; i < jobs.size(); i++)
	{
		jobArray[i].due_date = jobs[i].due_date;
		jobArray[i].proc_time = jobs[i].proc_time;
		t += jobs[i].proc_time;
	}
	PSUM = t;
	current_config.D = PSUM / 2;
	return true;
}

Configure DataReader2::get_current_configure()
{
	return configs[current_case - 1];
}

int DataReader2::get_current_case()
{
	return current_case;
}
