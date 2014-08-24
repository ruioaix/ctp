.PHONY : all install clean $(dirsname) 

all : *.h *.c 
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c mduserapi.cpp
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c ctpcapi.cpp
	g++ -shared -o libctpcapi.so mduserapi.o ctpcapi.o
	gcc -Wall -Wunused -c main.c
	g++ -Wall -Wunused -lthostmduserapi -L. -lctpcapi -lm main.o -o run
	

clean :
	@$(RM) a.out *.o run


