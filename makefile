all: 
	gcc -o schedulingAlgorithms schedulingAlgorithms.c
	gcc posix-rt.c -o posix-rt -lpthread -w
