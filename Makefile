.PHONY : all install clean $(dirsname)  test mdserver tdserver server_basic

#tdserver :

mdserver :  buildcapi server_basic 
	gcc -g -Wall -Wunused -c server/mds.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/mds.o
	gcc -g -Wall -Wunused -c server/mdcallback.c -I. -o bin/mdcallback.o
	g++ -g -Wall -Wunused bin/mds.o bin/vbmal.o bin/mdcallback.o  -L. -lctpcapi -lthostmduserapi -lrt -lmongoc-1.0 -lbson-1.0 -pthread -lm  -o mds
	
tdserver :  buildcapi server_basic
	gcc -g -Wall -Wunused -c server/tds.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/tds.o
	g++ -g -Wall -Wunused -lpthread  -lthostmduserapi -lthosttraderapi -lrt -lmongoc-1.0 -lbson-1.0 -L. -lm -lctpcapi bin/vbmal.o bin/tds.o -o tds

buildcapi :
	g++ -g -Wall -fPIC -Wunused -c cpp2c/base.c -I. -o bin/base.o
	g++ -g -fPIC -Wall -Wunused -c -I. cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -g -fPIC -Wall -Wunused -Werror -c -I. cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -g -shared -o cpp2c/slib/libctpcapi.so bin/base.o bin/mduserapi.o bin/ctpcapi.o 
	cp cpp2c/slib/libctpcapi.so libctpcapi.so

server_basic :
	gcc -g -Wall -Wunused -c server/vbmal.c   -o bin/vbmal.o
	
test :  
	gcc -Wall -fPIC -Wunused -c cpp2c/base.c -I. -o bin/base.o
	g++ -std=c++11 -fPIC -Wall -Wunused -c -I. cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c -I. cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -shared -o libctpcapi.so bin/base.o bin/mduserapi.o bin/ctpcapi.o
	gcc -Wall -Wunused -c mdserver/test_OnRtnDMD.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/test.o
	g++ -Wall -Wunused -lpthread -lthostmduserapi -lrt -lmongoc-1.0 -lbson-1.0 -L. -lctpcapi -lm bin/test.o -o test
	
all : mdserver tdserver

clean :
	@$(RM) a.out bin/* run *.log *.o ./*.so *.con 1 test mds tds
