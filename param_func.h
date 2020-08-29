#ifndef TEMPERATURE_THU
#define TEMPERATURE_THU
double temperature();
int use_rate();
char *result_dir(char*);
char *result_name(char*);
void file_clean();
double ARMclock();
void datalog(int);
void output_csv(char*);
void make_std_graph(char*,int);
char *separate_basenamed(char*);
void CPU_temp_graph(char*,int);
void CPU_usage_graph(char*,int);
void CPU_freq_graph(char*,int);
int judge_opt(char*,int);
void help_option();
void make_separate_graph(char*,int);

#endif
