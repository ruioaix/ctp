.PHONY : all install clean $(dirsname) 

all :  
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c -I. cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c -I. cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -shared -o libctpcapi.so bin/mduserapi.o bin/ctpcapi.o
	gcc -Wall -Wunused -c mdserver/main.c -I. -o bin/main.o
	g++ -Wall -Wunused -lthostmduserapi -L. -lctpcapi -lm bin/main.o -o run
	

clean :
	@$(RM) a.out bin/* run *.log *.o ./*.so


