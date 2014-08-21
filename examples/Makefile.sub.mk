headers := -I ../../API/originAPI
libtraderapi := $(realpath ../../API/originAPI/slib/linux64/thosttraderapi.so)
libmduserapi := $(realpath ../../API/originAPI/slib/linux64/thostmduserapi.so)

exec := quotation trade
topexec := $(addprefix ../../$(notdir $(CURDIR))-,$(exec))

.PHONY : all clean

all : $(exec)

% : %.c 
	echo $(topexec);
	g++ $(headers) $< $(libmduserapi)  -o $@
	cp $@ $(patsubst %,../../$(notdir $(CURDIR))-%,$@)

#quotation : quotation.c 
	#g++ $(headers) $< $(libmduserapi) -o $@
	#cp $@ $(patsubst %,../../$(notdir $(CURDIR))-%,$@)


clean :
	@$(RM) $(exec) $(topexec)
