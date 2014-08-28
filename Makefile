.PHONY : all install clean $(dirsname)  test

test:

all :  
	g++ -std=c++11 -fPIC -Wall -Wunused -c -I. cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c -I. cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -shared -o libctpcapi.so bin/mduserapi.o bin/ctpcapi.o
	gcc -Wall -Wunused -c mdserver/main.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/main.o
	g++ -Wall -Wunused -lpthread -lthostmduserapi -lrt -lmongoc-1.0 -lbson-1.0 -L. -lctpcapi -lm bin/main.o -o run
	
test :  
	g++ -std=c++11 -fPIC -Wall -Wunused -c -I. cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c -I. cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -shared -o libctpcapi.so bin/mduserapi.o bin/ctpcapi.o
	gcc -Wall -Wunused -c mdserver/test_OnRtnDMD.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/test.o
	g++ -Wall -Wunused -lpthread -lthostmduserapi -lrt -lmongoc-1.0 -lbson-1.0 -L. -lctpcapi -lm bin/test.o -o test
	

clean :
	@$(RM) a.out bin/* run *.log *.o ./*.so *.con 1
