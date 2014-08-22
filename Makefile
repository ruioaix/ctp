.PHONY : all install clean $(dirsname) 

all : *.h *.c 
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c MdUserApi.cpp
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c capi.cpp
	g++ -shared -o libctpcapi.so MdUserApi.o capi.o
	gcc -Wall -Wunused -c main.c
	g++ -Wall -Wunused  -L. -lthostmduserapi -lctpcapi -lm main.o -o run
	

clean :
	@$(RM) a.out *.o


