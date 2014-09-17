.PHONY : all install clean buildcapi server_basic run

#compile in c way
#but in the last step, because of libctpcapi.so, the program has to be linked in c++ way.
run :  buildcapi server_basic 
	gcc -g -Wall -Wunused -c server/main.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/main.o
	g++ -g -Wall -Wunused bin/main.o bin/vbmal.o bin/mongoapi.o -L. -lctpcapi -lthostmduserapi -lthosttraderapi -lrt -lmongoc-1.0 -lbson-1.0 -pthread -lm  -o run
	
#compile in c++ way, generate a .so libraries.
buildcapi :
	g++ -g -Wall -Wunused -fPIC -I. -c cpp2c/base.cpp -o bin/base.o
	g++ -g -Wall -Wunused -fPIC -I. -c cpp2c/mduserapi.cpp -o bin/mduserapi.o
	g++ -g -Wall -Wunused -fPIC -I. -c cpp2c/traderapi.cpp -o bin/traderapi.o
	g++ -g -Wall -Wunused -fPIC -I. -c cpp2c/ctpcapi.cpp -o bin/ctpcapi.o
	g++ -g -shared -o cpp2c/slib/libctpcapi.so bin/base.o bin/traderapi.o bin/mduserapi.o bin/ctpcapi.o 
	cp cpp2c/slib/libctpcapi.so libctpcapi.so

#compile in c way.
server_basic :
	gcc -g -Wall -Wunused -c server/vbmal.c -o bin/vbmal.o
	gcc -g -Wall -Wunused -c server/mongoapi.c -I. -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0  -o bin/mongoapi.o
	
all : run

clean :
	@$(RM) a.out bin/* run *.log *.o ./*.so *.con 1 test main tds

install : 
	sudo cp cpp_api/thostmduserapi.so /usr/lib64/
	sudo cp cpp_api/thosttraderapi.so /usr/lib64/
