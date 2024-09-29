all: My_Pub

My_Pub: My_Pub.o
	gcc -o My_Pub My_Pub.o -lpaho-mqtt3cs
	
My_Pub.o: My_Pub.c
	gcc -c -o My_Pub.o My_Pub.c
	
clean:
	rm -rf *.o My_Pub
