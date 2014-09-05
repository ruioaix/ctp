.PHONY : all install clean $(dirsname)  test mdserver tdserver vbmal

mdserver :  buildcapi vbmal
	gcc -Wall -Wunused -c server/mds.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/mds.o
	gcc -Wall -Wunused -c server/mdcallback.c -I. -o bin/mdcallback.o
	g++ -Wall -Wunused -lpthread -lthostmduserapi -lrt -lmongoc-1.0 -lbson-1.0 -L. -lctpcapi -lm bin/vbmal.o bin/mdcallback.o bin/mds.o -o mds
	
tdserver :  buildcapi vbmal
	gcc -Wall -Wunused -c server/tds.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/tds.o
	g++ -Wall -Wunused -lpthread -lthostmduserapi -lrt -lmongoc-1.0 -lbson-1.0 -L. -lctpcapi -lm bin/vbmal.o bin/tds.o -o tds

buildcapi :
	g++ -Wall -fPIC -Wunused -c cpp2c/base.c -I. -o bin/base.o
	g++ -std=c++11 -fPIC -Wall -Wunused -c -I. cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -std=c++11 -fPIC -Wall -Wunused -c -I. cpp2c/traderapi.cpp -o bin/traderapi.o
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c -I. cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -shared -o cpp2c/slib/libctpcapi.so bin/base.o bin/mduserapi.o bin/traderapi.o bin/ctpcapi.o 
	cp cpp2c/slib/libctpcapi.so libctpcapi.so

vbmal :
	gcc -Wall -Wunused -c server/vbmal.c   -o bin/vbmal.o
	
test :  
	gcc -Wall -fPIC -Wunused -c cpp2c/base.c -I. -o bin/base.o
	g++ -std=c++11 -fPIC -Wall -Wunused -c -I. cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -std=c++11 -fPIC -Wall -Wunused -Werror -c -I. cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -shared -o libctpcapi.so bin/base.o bin/mduserapi.o bin/ctpcapi.o
	gcc -Wall -Wunused -c mdserver/test_OnRtnDMD.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/test.o
	g++ -Wall -Wunused -lpthread -lthostmduserapi -lrt -lmongoc-1.0 -lbson-1.0 -L. -lctpcapi -lm bin/test.o -o test
	
all : mdserver tdserver

clean :
	@$(RM) a.out bin/* run *.log *.o ./*.so *.con 1 test
