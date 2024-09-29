all: My_Sub

My_Sub: My_Sub.o
	gcc -o My_Sub My_Sub.o -lpaho-mqtt3cs
	
My_Sub.o: My_Sub.c pubsub_opts.h
	gcc -c -o My_Sub.o My_Sub.c
	
clean:
	rm -rf *.o My_Sub
