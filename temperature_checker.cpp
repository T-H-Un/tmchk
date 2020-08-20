#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include "param_func.h" 
 

int main(int argc, char **argv){
	int i;
	char *p;
	char buf[4096];
	char buf2[2048];
	
	printf("\ntmchk ver. 0.02a powerd by T-H-Un\n");
	if(argc!=4&&argc!=5){
		printf("Error code 1 :unexpected figure of arguments: tmchk needs 3 or 4 cmd arguments\nusage:\n tmchk [sampling_rate(int/sec)] [logging_time(int/min)] [output png_path(full_path) ] (--separate)\n");
		return -1;
	}
	if(argc==5){
	if(strcmp(argv[4],"--separate")==0){printf("option : --separate ");}
	else{
	printf("Error code 2 : invalid argument: tmchk needs 3 or 4 cmd arguments\nusage:\n tmchk [sampling_rate(int/sec)] [logging_time(int/min)] [output png_path(full_path) ] (--separate)\nerror code 1:first argument need [int/sec]\n return -1\n");
	return -1;
	}	
		}
	int rate=atoi(argv[1]);
	int time=atoi(argv[2]);
	if(rate<1){
		printf("Error code 3 :invalid argument: tmchk needs 3 or 4 cmd arguments\nusage:\n tmchk [sampling_rate(int/sec)] [logging_time(int/min)] [output png_path(full_path) ] (--separate)\nerror :first argument need [int/sec]\n return -1\n");
		return -1;
		}
	file_clean();
	time= time*60;
	sprintf(buf2,argv[3]);
	if(buf2[0]!='/'){
		printf("\nError: code 4\n\ntmchk is need full path of png file.\n");
		printf("invalid argument: tmchk needs 3 or 4 cmd arguments\nusage:\n tmchk [sampling_rate(int/sec)] [logging_time(int/min)] [output png_path(full_path) ] (--separate)\nerror code 1:first argument need [int/sec]\n return -1\n");
		return -1;
	}
	p=(char*)buf2;
	printf("\noutput file path -> %s\n\n",buf2);
	for (i=0;i*rate<time;i++){
		std::thread data(datalog,rate*i);
		sleep(rate);
		data.join();
	}
	printf("\nend writing -> /tmp/data.thu\n");
	if(argc==4){
	make_std_graph(argv[3],rate*i);
	printf("\noutput 1 graph\nBye.");
	}
	
	if(argc==5){
	sprintf(buf,"%s_CPU_temperature.png",separate_basenamed(argv[3]));
	p=(char*)buf;
	CPU_temp_graph(p,rate*i);
	sprintf(buf,"%s_CPU_usage.png",separate_basenamed(argv[3]));
	p=(char*)buf;
	CPU_usage_graph(p,rate*i);
	sprintf(buf,"%s_CPU_frequency.png",separate_basenamed(argv[3]));
	p=(char*)buf;
	CPU_freq_graph(p,rate*i);
	make_std_graph(argv[3],rate*i);
	printf("\noutput 4 graphs/n Bye.\n");
	}
	sleep(1);
	file_clean();
	return 0;
}
