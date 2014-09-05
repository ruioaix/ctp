.PHONY : all install clean $(dirsname)  test

all :  
	g++ -Wall -fPIC -Wunused -c cpp2c/base.c -I. -o bin/base.o
	g++ -std=c++11 -fPIC -Wall -Wunused -c -I. cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c -I. cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -shared -o cpp2c/slib/libctpcapi.so bin/base.o bin/mduserapi.o bin/ctpcapi.o 
	cp cpp2c/slib/libctpcapi.so libctpcapi.so
	gcc -Wall -Wunused -c mdserver/main.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/main.o
	gcc -Wall -Wunused -c mdserver/mdcallback.c -I. -o bin/mdcallback.o
	gcc -Wall -Wunused -c mdserver/vbmal.c   -o bin/vbmal.o
	g++ -Wall -Wunused -lpthread -lthostmduserapi -lrt -lmongoc-1.0 -lbson-1.0 -L. -lctpcapi -lm bin/vbmal.o bin/mdcallback.o bin/main.o -o run
	
test :  
	gcc -Wall -fPIC -Wunused -c cpp2c/base.c -I. -o bin/base.o
	g++ -std=c++11 -fPIC -Wall -Wunused -c -I. cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c -I. cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -shared -o libctpcapi.so bin/base.o bin/mduserapi.o bin/ctpcapi.o
	gcc -Wall -Wunused -c mdserver/test_OnRtnDMD.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/test.o
	g++ -Wall -Wunused -lpthread -lthostmduserapi -lrt -lmongoc-1.0 -lbson-1.0 -L. -lctpcapi -lm bin/test.o -o test
	

clean :
	@$(RM) a.out bin/* run *.log *.o ./*.so *.con 1 test
