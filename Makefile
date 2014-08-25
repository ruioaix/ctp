all : trade quotation

trade : trade.c 
	g++ $< -lthosttraderapi -g -o $@

quotation : quotation.c
	g++ $< -lthostmduserapi -g -o $@
	

.PHONY : clean all 

clean : 
	@$(RM) *.o *.con a.out trade quotation
