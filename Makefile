.PHONY : all install clean buildcapi server_basic run

#tdserver :

run :  buildcapi server_basic 
	gcc -g -Wall -Wunused -c server/main.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/main.o
	g++ -g -Wall -Wunused bin/main.o bin/vbmal.o -L. -lctpcapi -lthostmduserapi -lthosttraderapi -lrt -lmongoc-1.0 -lbson-1.0 -pthread -lm  -o run
	
buildcapi :
	g++ -g -Wall -fPIC -Wunused -c cpp2c/base.c -I. -o bin/base.o
	g++ -g -fPIC -Wall -Wunused -c -I. cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -g -fPIC -Wall -Wunused -c -I. cpp2c/traderapi.cpp -o bin/traderapi.o
	g++ -g -fPIC -Wall -Wunused -Werror -c -I. cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -g -shared -o cpp2c/slib/libctpcapi.so bin/base.o bin/traderapi.o bin/mduserapi.o bin/ctpcapi.o 
	cp cpp2c/slib/libctpcapi.so libctpcapi.so

server_basic :
	gcc -g -Wall -Wunused -c server/vbmal.c   -o bin/vbmal.o
	
all : run

clean :
	@$(RM) a.out bin/* run *.log *.o ./*.so *.con 1 test main tds

install : 
	sudo cp cpp_api/thostmduserapi.so /usr/lib64/
	sudo cp cpp_api/thosttraderapi.so /usr/lib64/
