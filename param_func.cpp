#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define RASPBIAN 

double temperature(){
	
	FILE* file;
	char buffer[128];
	
	file=fopen("/sys/class/thermal/thermal_zone0/temp","r");
	fgets(buffer, 128, file);
	fclose(file);
	buffer[strlen(buffer)-1]='\0';
	int num=atoi(buffer);
	double temperature=num/1000;
	return temperature;
	}
	
int use_rate(){
	FILE *file;
	char buffer[1024];
	char buf[128];
	char *pcmd;
	int i=1;
	int c=0;	
	file=popen("top -b -n 1","r");
	if (file==NULL){return -1;}
	while(fgets(buffer, 1024, file) != NULL){
		if(i==3){
			pcmd = strtok(buffer, ",");
			while(c!=3){	
				pcmd = strtok(NULL, ",");
				c++;
				}
			strcpy(buf,pcmd);
			buf[strlen(buf)-2]='\0';
			break;
		}
		else{}
		i++;
	}
	pclose(file);
	int num=atoi(buf);
	int per=100-num;
	return per;
}
	
char *result_dir(char *a){
	char buffer[1024],buf[1024];
	strcpy(buf,a);
	char* p = strrchr( a, '/' );
	strcpy(buffer,p);
	buf[strlen(buf)-strlen(buffer)]='\0';
	p=(char*)buf;
	return p;
}

char *result_name(char *a){
	char* p = strrchr( a, '/' );
	char buf[1024];
	int i;
	strcpy(buf,p);
	for(i=0;buf[i+1]!='\0';i++){
		buf[i]=buf[i+1];
	}
	buf[i]='\0';
	p=(char*)buf;
	return p;
}

void file_clean(){
	remove("/tmp/data.csv");
	remove("/tmp/rttemp.cmd");
	remove("/tmp/rtusage.cmd");
	remove("/tmp/rtfreq.cmd");
}


#ifdef RASPBIAN
double ARMclock(){
	FILE *file;
	char buffer[128];
	char buf[128];
	char *pcmd;	
	
	file=popen("vcgencmd measure_clock arm","r");
	if (file==NULL){return -1;}
	fgets(buffer, 128, file);
	pclose(file);
	buffer[strlen(buffer)-1]='\0';	
	pcmd = strtok(buffer, "=");
	pcmd = strtok(NULL, "=");
	strcpy(buf,pcmd);
	int num=atoi(buf);
	double clock=num/1000000;
	return clock;
}

#endif
char *separate_basenamed(char *argv){
	char buf[2048];
	strcpy(buf,argv);
	buf[strlen(buf)-3]='\0';
	argv=(char*)buf;
	return argv;
}

void CPU_temp_graph(char *name,int time){
	FILE *file;
	
	printf("start gnuplot : CPU temperature graph\n");
	file = popen("gnuplot\n","w");
	fprintf(file, "set datafile separator \",\"\n");
	fprintf(file, "set terminal png size 1280,720\n");
	fprintf(file, "set output \"%s\"\n",name);
	fprintf(file, "set multiplot\n");
	fprintf(file, "unset key\n");
	fprintf(file, "set xrange [0:%d]\n",time);
	fprintf(file, "set xlabel \"time(sec)\"\n");
	fprintf(file, "set yrange [0:100.2]\n");
	fprintf(file, "set ytics 10\n");
	fprintf(file, "set ylabel \"temperature(deg)\" textcolor rgb \"red\"\n");
	fprintf(file, "set title \"CPU temperature\"\n");
	fprintf(file, "plot \'/tmp/data.csv\' using 1:2 with lines lt rgb \'red\'\n");
	fprintf(file, "unset terminal\n");
	fprintf(file, "unset multiplot\n");
	fprintf(file, "exit\n");
	pclose(file);
	}
	
void CPU_freq_graph(char *name,int time){
	FILE *file;
	
	printf("start gnuplot : CPU clock graph\n");
	file = popen("gnuplot\n","w");
	fprintf(file, "set datafile separator \",\"\n");
	fprintf(file, "set terminal png size 1280,720\n");
	fprintf(file, "set output \"%s\"\n",name);
	fprintf(file, "set multiplot\n");
	fprintf(file, "unset key\n");
	fprintf(file, "set xrange [0:%d]\n",time);
	fprintf(file, "set xlabel \"time(sec)\"\n");
	fprintf(file, "set title \"CPU frequency\"\n");
	fprintf(file, "set ytics 250\n");
	fprintf(file, "set yrange [0:2505]\n");
	fprintf(file, "set ylabel \"CPU frequency (MHz)\" textcolor rgb \"green\"\n");
	fprintf(file, "plot \'/tmp/data.csv\' using 1:4 with lines lt rgb \'green\'\n");
	fprintf(file, "unset terminal\n");
	fprintf(file, "unset multiplot\n");
	fprintf(file, "exit\n");
	pclose(file);
}	

void CPU_usage_graph(char *name,int time){
	FILE *file;
	
	printf("start gnuplot : CPU usage graph\n");
	file = popen("gnuplot\n","w");
	fprintf(file, "set datafile separator \",\"\n");
	fprintf(file, "set terminal png size 1280,720\n");
	fprintf(file, "set output \"%s\"\n",name);
	fprintf(file, "set multiplot\n");
	fprintf(file, "unset key\n");
	fprintf(file, "set xrange [0:%d]\n",time);
	fprintf(file, "set xlabel \"time(sec)\"\n");
	fprintf(file, "set title \"CPU usage\"\n");
	fprintf(file, "set ytics 10\n");
	fprintf(file, "set yrange [0:100.2]\n");
	fprintf(file, "set ylabel \"CPU usage (%%)\" textcolor rgb \"blue\"\n");
	fprintf(file, "plot \'/tmp/data.csv\' using 1:3 with lines lt rgb \'blue\'\n");
	fprintf(file, "unset terminal\n");
	fprintf(file, "unset multiplot\n");
	fprintf(file, "exit\n");
	pclose(file);
}

void datalog(int a){
	FILE *file;
	file = fopen("/tmp/data.csv", "a");
	if(file==NULL){
		printf("Can't open(or make) data.thu\nPlease check directory and permission./n");
	}
	printf("time=%d\ntemperature=%.2f(deg),  CPU_usage=%d(%%),  ARMclock=%.2f(MHz)\n",a,temperature(),use_rate(),ARMclock());
	fprintf(file,"%d,%.2f,%d,%.2f\n",a,temperature(),use_rate(),ARMclock());
	fclose(file);
}

void output_csv(char *argv){
	FILE *csv;
	FILE *data;
	char buf[1024];
	char log[128];
	strcpy(buf,argv);
	buf[strlen(buf)-3]='\0';
	strcat(buf,"csv");
	remove(buf);
	csv = fopen(buf,"a");
	data = fopen("/tmp/data.csv","r");
	fprintf(csv,"time,CPU_temperature,CPU_usage,CPU_frequency\n");
	 while((fgets(log,128,data))!=NULL){
		fprintf(csv,log);
	}
	fclose(csv);
	fclose(data);
	printf("output csv file. \noutput directory => %s \n ",result_dir(argv));
}

void make_std_graph(char *argv,int time){
	FILE *file;
	
	printf("start gnuplot : All System performance\n");
	file = popen("gnuplot\n","w");
	fprintf(file, "set datafile separator \",\"\n");
	fprintf(file, "set terminal png size 1280,720\n");
	fprintf(file, "set output \"%s\"\n",argv);
	fprintf(file, "set multiplot\n");
	fprintf(file, "unset key\n");
	fprintf(file, "set xrange [0:%d]\n",time);
	fprintf(file, "set xlabel \"time(sec)\"\n");
	fprintf(file, "set lmargin screen 0.15\n");
	fprintf(file, "set yrange [0:100.2]\n");
	fprintf(file, "set ytics 10\n");
	fprintf(file, "set ylabel \"temperature(deg)\" offset 2,0 textcolor rgb \"red\"\n");
	fprintf(file, "set title \"System Performance\"\n");
	fprintf(file, "plot \'/tmp/data.csv\' using 1:2 with lines lt rgb \'red\'\n");
	fprintf(file, "set ytics 10\n");
	fprintf(file, "set yrange [0:100.2]\n");
	fprintf(file, "set ytics offset -5,0\n");
	fprintf(file, "set ylabel \"CPU usage (%%)\" offset -3,0 textcolor rgb \"blue\"\n");
	fprintf(file, "plot \'/tmp/data.csv\' using 1:3 with lines lt rgb \'blue\'\n");
	fprintf(file, "set ytics 250\n");
	fprintf(file, "set yrange [0:2505]\n");
	fprintf(file, "set ytics offset -10,0\n");
	fprintf(file, "set ylabel \"CPU frequency (MHz)\" offset -8,0 textcolor rgb \"green\"\n");
	fprintf(file, "plot \'/tmp/data.csv\' using 1:4 with lines lt rgb \'green\'\n");
	fprintf(file, "unset terminal\n");
	fprintf(file, "unset multiplot\n");
	fprintf(file, "exit\n");
	pclose(file);
	
	}
	
void make_separate_graph(char *argv,int rate){
	char buf[4096];
	char *p;
	sprintf(buf,"%s_CPU_temperature.png",separate_basenamed(argv));
	p=(char*)buf;
	CPU_temp_graph(p,rate);
	sprintf(buf,"%s_CPU_usage.png",separate_basenamed(argv));
	p=(char*)buf;
	CPU_usage_graph(p,rate);
	sprintf(buf,"%s_CPU_frequency.png",separate_basenamed(argv));
	p=(char*)buf;
	CPU_freq_graph(p,rate);
	make_std_graph(argv,rate);
	printf("\noutput 4 graphs.\noutput directory => %s \n",result_dir(argv));
}



int judge_opt(char* argv,int flag){
		if(strcmp(argv,"--separate")==0){// flag 1
			printf(" --separate");
			if(flag==0)
				flag=1;
			else if(flag==1)
				flag=2583;
			else if(flag==2)
				flag=12;
			else if(flag==3)
				flag=3;
			else if(flag==23)
				flag=3;
		}
		else if(strcmp(argv,"--nocsv")==0){//flag 2
			printf(" --nocsv");
			if(flag==0)
				flag=2;
			else if(flag==1)
				flag=12;
			else if(flag==2)
				flag=2583;
			else if(flag==3)
				flag=3;
		}
		else if(strcmp(argv,"--nograph")==0){ //flag 3
			printf(" --nograph");
			if(flag!=3)
				flag=3;
			else if(flag==93)
				flag=93;
			else{
			flag=2583;
			}	
		}
		else if(strcmp(argv,"--realtime_all")==0){
			printf(" --realtime_all");
		}
		else if(strcmp(argv,"--realtime_temp")==0){
			printf(" --realtime_temp");
		}
		else if(strcmp(argv,"--realtime_usage")==0){
			printf(" --realtime_usage");
		}
		else if(strcmp(argv,"--realtime_freq")==0){
			printf(" --realtime_freq");
		}
		else{
			flag=93;
		}
	// flag 1 2 3 12 93 2583
	return flag;
}

int judge_rt(char* argv,int rt_flag){	//All=1000 -> 8, temperature = 0100 -> 4 usage = 0010 -> 2 , freq = 0001 -> 1
	 if(strcmp(argv,"--realtime_all")==0){
		if(rt_flag==0)
		rt_flag=8;
		else if(rt_flag!=0&&rt_flag<8)
		rt_flag=93;
		else
		rt_flag=2583;
	}
	else if(strcmp(argv,"--realtime_temp")==0){
		if(rt_flag==0)
		rt_flag=4;
		else if(rt_flag==1)
		rt_flag=5;
		else if(rt_flag==2)
		rt_flag=6;
		else if(rt_flag==3)
		rt_flag=7;
		else if(rt_flag==8)
		rt_flag=93;
		else if(rt_flag==93)
		;
		else
		rt_flag=2583;
	}
	else if(strcmp(argv,"--realtime_usage")==0){
		if(rt_flag==0)
		rt_flag=2;
		else if(rt_flag==4)
		rt_flag=6;
		else if(rt_flag==1)
		rt_flag=3;
		else if(rt_flag==5)
		rt_flag=7;
		else if(rt_flag==8)
		rt_flag=93;
		else if(rt_flag==93)
		;
		else
		rt_flag=2583;
	}
	else if(strcmp(argv,"--realtime_freq")==0){
		if(rt_flag==0)
		rt_flag=1;
		else if(rt_flag==4)
		rt_flag=5;
		else if(rt_flag==2)
		rt_flag=3;
		else if(rt_flag==6)
		rt_flag=7;
		else if(rt_flag==8)
		rt_flag=93;
		else if(rt_flag==93)
		;
		else
		rt_flag=2583;
	}
	else;
	return rt_flag;
}

void help_option(){
	printf("\nusage:\n tmchk [sampling_rate(int/sec)] [logging_time(int/min)] [output png_path(full_path) ] (--options)\n");
	printf("\nOptions\n--separate : Create one graph for one parameter.\n");
	printf("--nocsv : Don't output CSV file.\n");
	printf("--nograph : Don't create graphs.\n");
	printf("--realtime_all : Drawing 3 realtime graphs(CPU temperature,CPU usage,CPU frequency)\n");
	printf("--realtime_temp : Drawing realtime graph about CPU temperature.\n");
	printf("--realtime_usage : Drawing realtime graph about CPU usage.\n");
	printf("--realtime_freq : Drawing realtime graph about CPU frequency.\n");
}

void realtime_temp_graph(int rate){
	FILE *gnuplot;
	gnuplot = fopen("/tmp/rttemp.cmd","a");
	fprintf(gnuplot,"set datafile separator \",\"\nunset key\nset xlabel \"time(sec)\"\nset yrange [0:100.2]\nset ylabel \"temperature(deg)\" textcolor rgb \"red\"\nset title \"CPU temperature\"\nplot \'/tmp/data.csv\' using 1:2 with lines lt rgb \'red\'\npause %d\nreread\n",rate);
	fclose(gnuplot);
	sleep(rate);
	gnuplot=popen("gnuplot /tmp/rttemp.cmd", "w"); 
	pclose(gnuplot);
}
void realtime_usage_graph(int rate){
	FILE *gnuplot;
	gnuplot = fopen("/tmp/rtusage.cmd","a");
	fprintf(gnuplot,"set datafile separator \",\"\nunset key\nset xlabel \"time(sec)\"\nset yrange [0:100.2]\nset ylabel \"usege(%%)\" textcolor rgb \"blue\"\nset title \"CPU usage\"\nplot \'/tmp/data.csv\' using 1:3 with lines lt rgb \'blue\'\npause %d\nreread\n",rate);
	fclose(gnuplot);
	sleep(rate);
	gnuplot=popen("gnuplot /tmp/rtusage.cmd", "w"); 
	pclose(gnuplot);
}
void realtime_freq_graph(int rate){
	FILE *gnuplot;
	gnuplot = fopen("/tmp/rtfreq.cmd","a");
	fprintf(gnuplot,"set datafile separator \",\"\nunset key\nset xlabel \"time(sec)\"\nset yrange [0:2500]\nset ylabel \"frequency(MHz)\" textcolor rgb \"green\"\nset title \"CPU frequency\"\nplot \'/tmp/data.csv\' using 1:4 with lines lt rgb \'green\'\npause %d\nreread\n",rate);
	fclose(gnuplot);
	sleep(rate);
	gnuplot=popen("gnuplot /tmp/rtfreq.cmd", "w"); 
	pclose(gnuplot);
}
