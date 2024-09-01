all: Kurs

Kurs: mainANDinit.o inputANDconrollUserDron.o AIcontrolAutoDron.o HelpFunction.o
	gcc -o kurs mainANDinit.o inputANDconrollUserDron.o AIcontrolAutoDron.o HelpFunction.o

mainANDinit.o: mainANDinit.c HeaderFail.h
	gcc  -c -o mainANDinit.o mainANDinit.c

inputANDconrollUserDron.o: inputANDconrollUserDron.c
	gcc  -c -o inputANDconrollUserDron.o inputANDconrollUserDron.c

AIcontrolAutoDron.o: AIcontrolAutoDron.c
	gcc  -c -o AIcontrolAutoDron.o AIcontrolAutoDron.c

HelpFunction.o: HelpFunction.c
	gcc  -c -o HelpFunction.o HelpFunction.c
	
clean:
	del *.o 
	del Kurs.exe
