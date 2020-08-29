#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include "param_func.h" 
#define OPT_NUM 3

int main(int argc, char **argv){
	int i;
	int flag=0;
	char *p;
	char buf2[2048];
	
	printf("\ntmchk ver. 0.04a powerd by T-H-Un\n");
	if(strcmp(argv[1],"--help")==0){
		help_option();
		return -1;
	}
	if(argc>OPT_NUM+4||argc<=3){
		printf("\nError code 1 :unexpected figure of arguments\n");
		help_option();
		return -1;
	}
	printf("option : ");
	for(i=4;i<argc;i++){
	flag=judge_opt(argv[i],flag);
	}
	printf("\n");
	if(flag==2583){
		printf("\nError code 2 :use same options.\n");
		help_option();
		return -1;
	}
	int rate=atoi(argv[1]);
	int time=atoi(argv[2]);
	if(rate<1){
		printf("\nError code 3 :invalid argument\n");
		help_option();
		return -1;
		}
	file_clean();
	
	time= time*60;
	sprintf(buf2,argv[3]);
	if(buf2[0]!='/'){
		printf("\nError: code 4\n\ntmchk is need full path of png file.\n");
		help_option();
		return -1;
	}
	p=(char*)buf2;
	printf("\noutput file path -> %s\n\n",p);
	for (i=0;i*rate<time;i++){
		std::thread data(datalog,rate*i);
		sleep(rate);
		data.join();
	}
	printf("\nend writing -> /tmp/data.csv\n");
	if(flag==0||flag==2){
	make_std_graph(argv[3],rate*i);
	printf("\noutput 1 graph and data csv file. \noutput directory => %s \n",result_dir(argv[3]));
	}
	
	else if(flag==1||flag==12){
	make_separate_graph(argv[3],rate*i);
	}
	
	if(flag==1||flag==3){
		output_csv(argv[3]);
	}
	
	if(flag==2||flag==12){
		
	}
	printf("Bye.\n");
	sleep(1);
	file_clean();
	return 0;
}
