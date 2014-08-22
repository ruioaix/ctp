.PHONY : all install clean $(dirsname) 

all : *.h *.c 
	g++ -std=c++11 -Wall -Wunused -Werror -c cmain.c
	g++ -std=c++11 -Wall -Wunused -Werror -fpic -c MdUserApi.cpp
	g++ -std=c++11 -Wall -Wunused -Werror -fpic -c toolkit.cpp
	g++ -std=c++11 -Wall -Wunused -Werror -fpic -c capi.cpp
	g++ --shared -o libctpcapi.so MdUserApi.o toolkit.o capi.o
	gcc -Wall -Wunused -c main.c
	gcc -Wall -Wunused -c callback.c
	g++ -Wall -Wunused  -L. -lthostmduserapi -lctpcapi main.o callback.o -o run
	

clean :
	@$(RM) a.out *.o


