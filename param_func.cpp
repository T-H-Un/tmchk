#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define RASPBIAN //if you use on not raspbian, please comment out this line.
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
	char cmd[]="top -b -n 1 > /tmp/top.thu";
	char buffer[1024];
	char buf[128];
	char *pcmd;
	int i=1;
	int c=0;	
	pcmd=(char*)cmd;
	system(pcmd);
	file=fopen("/tmp/top.thu","r");
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
	fclose(file);
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
	remove("/tmp/top.thu");
	remove("/tmp/clock.thu");
	remove("/tmp/data.thu");
}


#ifdef RASPBIAN
double ARMclock(){
	FILE *file;
	char cmd[]="vcgencmd measure_clock arm > /tmp/clock.thu";
	char buffer[128];
	char buf[128];
	char *pcmd;	
	
	pcmd=(char*)cmd;
	system(pcmd);
	file=fopen("/tmp/clock.thu","r");
	if (file==NULL){return -1;}
	fgets(buffer, 128, file);
	fclose(file);
	buffer[strlen(buffer)-1]='\0';	
	pcmd = strtok(buffer, "=");
	pcmd = strtok(NULL, "=");
	strcpy(buf,pcmd);
	int num=atoi(buf);
	double clock=num/1000000;
	return clock;
}

#endif
void datalog(int a){
	FILE *file;
	file = fopen("/tmp/data.thu", "a");
	if(file==NULL){
		printf("Can't open(or make) data.thu\nPlease check directory and permission./n");
	}
	printf("time=%d\ntemperature=%.2f(deg),  CPU_usage=%d(%%),  ARMclock=%.2f(MHz)\n",a,temperature(),use_rate(),ARMclock());
	fprintf(file,"%d %.2f %d %.2f\n",a,temperature(),use_rate(),ARMclock());
	fclose(file);
}
