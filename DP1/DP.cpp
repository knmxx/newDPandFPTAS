#include<iostream>
#include <thread>
#include <mutex>
#include "Datareader2.h"
#include "Datareader3.h"
#include "Datareader4.h"
#include "DP21.h"
#include "DP23.h"
#include "DP31.h"
#include "DP33.h"
#include "DP41.h"
#include "DP43.h"
#include "FPTASDP21.h"
#include "FPTASDP23.h"
#include "FPTASDP24.h"
#include "FPTASDP31.h"
#include "FPTASDP33.h"
#include "FPTASDP34.h"
#include "FPTASDP41.h"
#include "FPTASDP43.h"
#include "Job.h"
using namespace std;


const int TASK_DP21 = 1, TASK_DP22 = 2, TASK_DP31 = 4, TASK_DP32 = 8, TASK_DP41 = 16, TASK_DP42 = 32;
timed_mutex mtx;
bool result_error = false;
void dump_table_header(FILE* fp)//print fields of table in csv
{
	fprintf(fp, "M, N, Name, Result, Time\n");
	fflush(fp);
}
void dump_data(const char* algorithm_name, int m, int n, double result, double running_time, FILE* fp)
{
	printf("%2d, %2d,  %-20s, %f,\t%.4f\n", m, n, algorithm_name, result, running_time);
	fprintf(fp, "%d, %d, %s, %f, %f\n", m, n, algorithm_name, result, running_time);
	fflush(fp);
}

void run_algorithms(FILE* result_file, int seconds, int flags)
{

	//运行哪些就把注释取消

	int dp2_latework = -1, dp_result22 = -1, dp3_latework = -1, dp_result32 = 1, dp4_latework = 1, dp_result42 = -1;
	printf("PSUM:%d\n", PSUM);
	if (flags & TASK_DP21)
	{
		//dp2_latework = dp21();
		//dump_data("原DP", current_config.m, current_config.n, PSUM - dp2_latework, dp2_running_time, result_file);
		////FPTAS
		//int dpe21_latework = dpe21();
		//if (dpe21_latework == dp2_latework)
		//	dump_data("原DP的FPTAS", current_config.m, current_config.n, 1, dpe21_running_time, result_file);
		//else
		//	dump_data("原DP的FPTAS", current_config.m, current_config.n, 1.0 * (PSUM - dp2_latework) / (PSUM - dpe21_latework), dpe21_running_time, result_file);
	}
	if (flags & TASK_DP22)
	{
		/*dp_result22 = dp22();
		dump_data("DP_xin", current_config.m, current_config.n, dp_result22, dp22_running_time, result_file);*/
		/*int newdp2_result = newdp2();
		dump_data("DP_Misha", current_config.m, current_config.n, newdp2_result, newdp2_running_time, result_file);*/
		int dp_result23 = dp23();
		/*dump_data("DP_Misha", current_config.m, current_config.n, dp_result23, dp23_running_time, result_file);*/
		//FPTAS
		//int fptas_result23 = fptas23();
		///*dump_data("FPTAS_Misha", current_config.m, current_config.n, fptas_result23, fptas23_running_time, result_file);*/
		//if (fptas_result23 == dp_result23)
		//	dump_data("Misha的FPTAS", current_config.m, current_config.n, 1, fptas23_running_time, result_file);
		//else
		//	dump_data("Misha的FPTAS", current_config.m, current_config.n, 1.0 * dp_result23 / fptas_result23, fptas23_running_time, result_file);

		//li  Fptas
		int fptas_result24 = fptas24();
		if (fptas_result24 == dp_result23)
			dump_data("Li的FPTAS", current_config.m, current_config.n, 1, fptas24_running_time, result_file);
		else
			dump_data("Li的FPTAS", current_config.m, current_config.n, 1.0 * dp_result23 / fptas_result24, fptas24_running_time, result_file);

	}
	if (flags & TASK_DP31)
	{
		//dp3_latework = dp31();
		//dump_data("DP原", current_config.m, current_config.n, PSUM - dp3_latework, dp3_running_time, result_file);
		////FPTAS
		//int dpe31_latework = dpe31();
		//if (dpe31_latework == dp3_latework)
		//	dump_data("原DP的FPTAS", current_config.m, current_config.n, 1, dpe31_running_time, result_file);
		//else
		//	dump_data("原DP的FPTAS", current_config.m, current_config.n, 1.0 * (PSUM - dp3_latework) / (PSUM - dpe31_latework), dpe31_running_time, result_file);
	}
	if (flags & TASK_DP32)
	{
		/*dp_result32 = dp32();
		dump_data("DP_xin", current_config.m, current_config.n, dp_result32, dp32_running_time, result_file);*/
		/*int newdp3_result = newdp3();
		dump_data("DP_Misha", current_config.m, current_config.n, newdp3_result, newdp3_running_time, result_file);*/
		int dp_result33 = dp33();
		dump_data("DP_Misha", current_config.m, current_config.n, dp_result33, dp33_running_time, result_file);
		/*int dp_result331 = dp331();
		dump_data("DP_Misha2", current_config.m, current_config.n, dp_result331, dp331_running_time, result_file);*/

		//FPTAS
		int fptas_result33 = fptas33();
		/*dump_data("FPTAS_Misha", current_config.m, current_config.n, fptas_result23, fptas23_running_time, result_file);*/
		if (fptas_result33 == dp_result33)
			dump_data("Misha的FPTAS", current_config.m, current_config.n, 1, fptas33_running_time, result_file);
		else
			dump_data("Misha的FPTAS", current_config.m, current_config.n, 1.0 * dp_result33 / fptas_result33, fptas33_running_time, result_file);

		//li  Fptas
		int fptas_result34 = fptas34();
		if (fptas_result34 == dp_result33)
			dump_data("Li的FPTAS", current_config.m, current_config.n, 1, fptas34_running_time, result_file);
		else
			dump_data("Li的FPTAS", current_config.m, current_config.n, 1.0 * dp_result33 / fptas_result34, fptas34_running_time, result_file);
	}
	if (flags & TASK_DP41)
	{
		dp4_latework = dp41();
		dump_data("DP原", current_config.m, current_config.n, PSUM - dp4_latework, dp4_running_time, result_file);
		////FPTAS
		//int dpe41_latework = dpe41();
		//if (dpe41_latework == dp4_latework)
		//	dump_data("原DP的FPTAS", current_config.m, current_config.n, 1, dpe41_running_time, result_file);
		//else
		//	dump_data("原DP的FPTAS", current_config.m, current_config.n, 1.0 * (PSUM - dp4_latework) / (PSUM - dpe41_latework), dpe41_running_time, result_file);
	}
	if (flags & TASK_DP42)
	{
		/*dp_result42 = dp42();
		dump_data("DP_xin", current_config.m, current_config.n, dp_result42, dp42_running_time, result_file);*/
		/*int newdp4_result = newdp4();
		dump_data("DP_Misha", current_config.m, current_config.n, newdp4_result, newdp4_running_time, result_file);*/
		int dp_result43 = dp43();
		dump_data("DP_Misha", current_config.m, current_config.n, dp_result43, dp43_running_time, result_file);

		//FPTAS
		int fptas_result43 = fptas43();
		/*dump_data("FPTAS_Misha", current_config.m, current_config.n, fptas_result23, fptas23_running_time, result_file);*/
		if (fptas_result43 == dp_result43)
			dump_data("Misha的FPTAS", current_config.m, current_config.n, 1, fptas43_running_time, result_file);
		else
			dump_data("Misha的FPTAS", current_config.m, current_config.n, 1.0 * dp_result43 / fptas_result43, fptas43_running_time, result_file);
	}
	
}
void start_algorithm2(const char* data_filename, const char* output_filename, int algorithms, int timeout_seconds)
{
	FILE* result_file;
	errno_t err = fopen_s(&result_file, output_filename, "w");
	if (err != 0)
	{
		printf("[DATAREADER]Cannot open %s\n", output_filename);
		return;
	}
	dump_table_header(result_file);
	DataReader2 reader(data_filename);

	while (reader.has_next())
	{
		vector<Job> cj = reader.next_case();
		run_algorithms(result_file, timeout_seconds, algorithms);
	}
	fclose(result_file);
}
void start_algorithm3(const char* data_filename, const char* output_filename, int algorithms, int timeout_seconds)
{
	FILE* result_file;
	errno_t err = fopen_s(&result_file, output_filename, "w");
	if (err != 0)
	{
		printf("[DATAREADER]Cannot open %s\n", output_filename);
		return;
	}
	dump_table_header(result_file);
	DataReader3 reader(data_filename);

	while (reader.has_next())
	{
		vector<Job> cj = reader.next_case();
		run_algorithms(result_file, timeout_seconds, algorithms);
	}
	fclose(result_file);
}
void start_algorithm4(const char* data_filename, const char* output_filename, int algorithms, int timeout_seconds)
{
	FILE* result_file;
	errno_t err = fopen_s(&result_file, output_filename, "w");
	if (err != 0)
	{
		printf("[DATAREADER]Cannot open %s\n", output_filename);
		return;
	}
	dump_table_header(result_file);
	DataReader4 reader(data_filename);

	while (reader.has_next())
	{
		vector<Job> cj = reader.next_case();
		run_algorithms(result_file, timeout_seconds, algorithms);
	}
	fclose(result_file);
}
int main(int argc, int* argv[])
{
	char data_filename[256] = "../2/2machine.txt";
	char output_filename[256] = "../DP_result.csv";
	start_algorithm2(data_filename, output_filename, TASK_DP21 | TASK_DP22, 20);
	//start_algorithm3(data_filename, output_filename, TASK_DP31 | TASK_DP32, 20);
	//start_algorithm4(data_filename, output_filename, TASK_DP41 | TASK_DP42, 20);
	system("pause");
	return 0;
}