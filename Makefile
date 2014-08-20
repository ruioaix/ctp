all : run

run : main.c *.h
	g++ $< lib/linux64/thostmduserapi.so -o $@

.PHONY : clean all 

clean : 
	$(RM) *.o *.con a.out run
