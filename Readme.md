layout explanation
1, ctpcapi.h 
	this is the basic c api.
	c program will need to include this file and link to three shared libraries: thostmduserapi.so & thosttraderapi.so & libctpcapi.so.
2, the directory cpp2c will be compiled into libctpcapi.so. And building into libctpcapi.so is the only usage of this directory.
3, cpp_api is the origin c++ api. when compiling cpp2c, this directory will be used.
4, the directory bin is temp folder. no need to worry.
5, Makefile is Makefile. you can take a look.
6, others is others. no need to worry.
7, the directory server: I use c api to build the md server and trader server.
8, tags is tags. no need to worry.
