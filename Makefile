all : trade

#trade : trade.c *.h
#	g++ $< -lthostmduserapi -lthosttraderapi -o $@
#
#quotation : quotation.c *.h
#	g++ $< -lthostmduserapi -o $@

trade : trade.o 
	gcc $< -lthosttraderapi -lstdc++ -lthostmduserapi -o $@
#	gcc $< -lthosttraderapi -lstdc++ -o $@

%.o : %.c
	g++ -c $< -o $@

.PHONY : clean all 

clean : 
	@$(RM) *.o *.con a.out trade quotation
