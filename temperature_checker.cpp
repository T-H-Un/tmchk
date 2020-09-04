#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include "param_func.h" 
#define OPT_NUM 7 //--separate --nograph --nocsv --realtime_all --realtime_temp --realtime_usage --realtime_freq
int gnuplot_flag=0;

int main(int argc, char **argv){
	int i;
	int flag=0;
	int rt_flag=0;
	char *p;
	char buf2[2048];
	
	printf("\ntmchk ver. 0.10a powerd by T-H-Un\n");
	if(strcmp(argv[1],"--help")==0){
		help_option();
		return -1;
	}
	if(argc>OPT_NUM+4||argc<=3){
		printf("\nError code 1 :unexpected figure of arguments\n");
		help_option();
		return -1;
	}
	printf("Using option : ");
	for(i=4;i<argc;i++){
	flag=judge_opt(argv[i],flag);
	rt_flag=judge_rt(argv[i],rt_flag);
	}
	printf("\n");
	if(flag==2583||rt_flag==2583){
		printf("\nError code 2 :use same options.\n");
		help_option();
		return -1;
	}
	if(flag==93){
		printf("\nError code 5 : include unknown option.\n");
		help_option();
		return -1;
	}
	if(rt_flag==93){
		printf("\n Now, you use --realtime_all and --realtime_xxx , so just use --realtime_all option.\n");
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
	if(rt_flag>0){
		if(rt_flag==8||rt_flag==93||rt_flag==7){
		std::thread rt1(realtime_temp_graph,rate);
		std::thread rt2(realtime_usage_graph,rate);
		std::thread rt3(realtime_freq_graph,rate);
		rt1.detach();
		rt2.detach();
		rt3.detach();
	}
		if(rt_flag==4||rt_flag==5||rt_flag==6){
			std::thread rt1(realtime_temp_graph,rate);
			rt1.detach();
		}
		if(rt_flag==2||rt_flag==6||rt_flag==3){
			std::thread rt2(realtime_usage_graph,rate);
			rt2.detach();
		}
		if(rt_flag==1||rt_flag==5||rt_flag==3){
			std::thread rt3(realtime_freq_graph,rate);
			rt3.detach();
		}
			for (i=0;i*rate<time;i++){
			std::thread data(datalog,rate*i);
			sleep(rate);
			data.join();
		}
		gnuplot_flag=1;
	}
	else{
		for (i=0;i*rate<time;i++){
			std::thread data(datalog,rate*i);
			sleep(rate);
			data.join();
		}
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
	file_clean();
	sleep(2);
	if(rt_flag>0){
			printf("\n\nplease ignore gnuplot error message.\n");
	}
	printf("Bye.\n");
	return 0;
}


