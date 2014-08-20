all : trade 

trade : trade.c *.h
	g++ $< lib/linux64/thostmduserapi.so -o $@

quoation : quoation.c *.h
	g++ $< lib/linux64/thostmduserapi.so -o $@

.PHONY : clean all 

clean : 
	@$(RM) *.o *.con a.out trade quoation
