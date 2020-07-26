#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include "param_func.h" 
 

int main(int argc, char **argv){
	int i;
	FILE *file;
	
	char buf2[2048];
	
	if(argc!=4){
		printf("unexpected arguments: tmchk needs 3 cmd arguments\nusage:\n tmchk [sampling_rate(int/sec)] [logging_time(int/min)] [output png_path(full_path) ] \n");
		return -1;
	}
	int rate=atoi(argv[1]);
	int time=atoi(argv[2]);
	if(rate<1){
		printf("invalid argument: tmchk needs 3 cmd arguments\nusage:\n tmchk [sampling_rate(int/sec)] [logging_time(int/min)] [output png_path(full_path) ]\nerror code 1:first argument need [int/sec]\n return -1\n");
		return -1;
		}
	time= time*60;
	sprintf(buf2,argv[3]);
	printf("\noutput file path -> %s\n\n",buf2);
	for (i=0;i*rate<time;i++){
		std::thread data(datalog,rate*i);
		sleep(rate);
		data.join();
	}
	printf("\nend writing -> /tmp/data.thu\n");
	
	printf("start gnuplot\n");
	file = popen("gnuplot\n","w");
	fprintf(file, "set terminal png size 1280,720\n");
	fprintf(file, "set output \"%s\"\n",argv[3]);
	fprintf(file, "set multiplot\n");
	fprintf(file, "unset key\n");
	fprintf(file, "set xrange [0:%d]\n",rate*i);
	fprintf(file, "set xlabel \"time(sec)\"\n");
	fprintf(file, "set lmargin screen 0.15\n");
	fprintf(file, "set yrange [0:100]\n");
	fprintf(file, "set ytics 10\n");
	fprintf(file, "set ylabel \"temperature(deg)\" offset 2,0 textcolor rgb \"red\"\n");
	fprintf(file, "set title \"System Performance\"\n");
	fprintf(file, "plot \'/tmp/data.thu\' using 1:2 with linespoints lt rgb \'red\'\n");
	fprintf(file, "set ytics 10\n");
	fprintf(file, "set yrange [0:100]\n");
	fprintf(file, "set ytics offset -5,0\n");
	fprintf(file, "set ylabel \"CPU usage (%%)\" offset -3,0 textcolor rgb \"blue\"\n");
	fprintf(file, "plot \'/tmp/data.thu\' using 1:3 with linespoints lt rgb \'blue\'\n");
	fprintf(file, "set ytics 250\n");
	fprintf(file, "set yrange [0:2500]\n");
	fprintf(file, "set ytics offset -10,0\n");
	fprintf(file, "set ylabel \"CPU frequency (MHz)\" offset -8,0 textcolor rgb \"green\"\n");
	fprintf(file, "plot \'/tmp/data.thu\' using 1:4 with linespoints lt rgb \'green\'\n");
	fprintf(file, "unset terminal\n");
	fprintf(file, "unset multiplot\n");
	fprintf(file, "exit\n");
	pclose(file);
	
	sleep(3);
	file_clean();
	return 0;
}
