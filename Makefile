all: Kurs

Kurs: report.o temp_function.o
	gcc -o kurs report.o temp_function.o

report.o: report.c temp_function.h
	gcc  -c -o report.o report.c

temp_function.o: temp_function.c
	gcc  -c -o temp_function.o temp_function.c
	
clean:
	del *.o 
	del Kurs.exe
