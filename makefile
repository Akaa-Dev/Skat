# makefile of the exercise "bidding at a skat game"

# Important notes: 
#    All "recipe lines" MUST start with a <tab> character!!!
#    This file has to be named "makefile".
#    To build the skat progam, just type "make skat".
#    "make clean" will remove object files and executable files.
# gcc -pthread -o skat skat-main.o skat-threads.o -lrt --the original line 12

#  solution based on threads and global variables:


skat : skat-main.o skat-threads.o
	gcc -pthread -o skat skat-main.o skat-threads.o
	

#  solution based on processes and global variables in a shared memory:
#skat : skat-main.o skat-shm.o
#	gcc  -o skat skat-main.o skat-shm.o -lrt 

#  solution based on processes and pipes:
#skat : skat-main.o skat-pipes.o
#	gcc -o skat skat-main.o skat-pipes.o

#  solution based on processes and message queues:
#skat : skat-main.o skat-queues.o
#	gcc -o skat skat-main.o skat-queues.o -lrt 

skat-threads.o : skat-threads.c skat-defs.h glob-defs.h
	gcc -c skat-threads.c

skat-shm.o : skat-shm.c skat-defs.h glob-defs.h
	gcc -c skat-shm.c

skat-main.o : skat-main.c skat-comm.h glob-defs.h
	gcc -c skat-main.c

skat-pipes.o : skat-pipes.c skat-defs.h glob-defs.h
	gcc -c skat-pipes.c

skat-queues.o : skat-queues.c skat-defs.h glob-defs.h
	gcc -c skat-queues.c
	
clean :
	rm skat skat-main.o skat-pipes.o skat-queues.o skat-threads.o skat-shm.o

