all : trade quotation

trade : trade.c 
	g++ $< -lthosttraderapi -o $@

quotation : quotation.c
	g++ $< -lthostmduserapi -o $@
	

.PHONY : clean all 

clean : 
	@$(RM) *.o *.con a.out trade quotation
