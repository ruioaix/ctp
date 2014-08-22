.PHONY : all install clean $(dirsname) 

all : *.h *.c 
	gcc -Wall -Wunused -c main.c
	g++ -std=c++11 -Wall -Wunused -c cmain.c
	g++ -std=c++11 -Wall -Wunused -c MdUserApi.cpp
	g++ -std=c++11 -Wall -Wunused -c toolkit.cpp

clean :
	@$(RM) a.out *.o


