.PHONY : all install clean $(dirsname) 

all : *.h *.c 
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c -I. cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c -I. cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -shared -o libctpcapi.so bin/mduserapi.o bin/ctpcapi.o
	gcc -Wall -Wunused -c main.c
	g++ -Wall -Wunused -lthostmduserapi -L. -lctpcapi -lm main.o -o run
	

clean :
	@$(RM) a.out bin/* run *.log *.o


