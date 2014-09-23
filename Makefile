.PHONY : all install clean buildcapi server_basic run

#link in c++ way, because of libctpcapi.so.
run :  buildcapi server_basic 
	g++ -g -Wall -Wunused bin/main.o bin/threadfunc.o bin/mongoapi.o bin/io.o bin/safe.o -L. -lctpcapi -lthostmduserapi -lthosttraderapi -lrt -lmongoc-1.0 -lbson-1.0 -lta_lib -pthread -lm  -o run
	
#compile in c++ way, generate a .so libraries.
buildcapi :
	g++ -g -Wall -Wunused -fPIC -I. -c cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -g -Wall -Wunused -fPIC -I. -c cpp2c/traderapi.cpp -o bin/traderapi.o
	g++ -g -Wall -Wunused -fPIC -I. -c cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -g -shared -o cpp2c/slib/libctpcapi.so bin/traderapi.o bin/mduserapi.o bin/ctpcapi.o 
	cp cpp2c/slib/libctpcapi.so libctpcapi.so

#compile in c way.
server_basic :
	gcc -g -Wall -Wunused -c server/main.c -I. -I/usr/local/include/ta-lib -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/main.o
	gcc -g -Wall -Wunused -c server/safe.c -o bin/safe.o
	gcc -g -Wall -Wunused -c server/io.c -o bin/io.o
	gcc -g -Wall -Wunused -c server/threadfunc.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0 -o bin/threadfunc.o
	gcc -g -Wall -Wunused -c server/mongoapi.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/mongoapi.o
	
all : run

clean :
	@$(RM) a.out bin/* run *.log *.o ./*.so *.con 1 test main tds

install : 
	sudo cp cpp_api/thostmduserapi.so /usr/lib64/
	sudo cp cpp_api/thosttraderapi.so /usr/lib64/
