CC = gcc
CFLAGS = -O4 -Wall -I/usr/local/include
DEST = /usr/local/bin
LDFLAGS = -L/usr/local/lib -lpthread -lrt -ldl -lstdc++
PROGRAM = tmchk


$(PROGRAM) : temperature_checker.o param_func.o
	gcc -o $(PROGRAM) temperature_checker.o param_func.o $(LDFLAGS)

temperature_checker.o : temperature_checker.cpp
	gcc -c temperature_checker.cpp $(LDFLAGS)

param_func.o : param_func.cpp
	gcc -c param_func.cpp $(LDFLAGS)

clean : 
	rm -f *.o *~ $(PROGRAM)
install : $(PROGRAM)
	install -s $(PROGRAM) $(DEST)
