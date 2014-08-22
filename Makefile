.PHONY : all install clean $(dirsname) 

all : *.h *.c 
	gcc -Wall -Wunused  -c main.c

clean :
	@$(RM) a.out *.o
