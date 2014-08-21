headers := -I $(realpath ../../API/originAPI)
libtraderapi := $(realpath ../../API/originAPI/slib/linux64/thosttraderapi.so)
libmduserapi := $(realpath ../../API/originAPI/slib/linux64/thostmduserapi.so)

sources := $(wildcard *.c)
execu := $(notdir $(basename $(sources)))
topexecu := $(addprefix ../../$(notdir $(CURDIR))-,$(execu))

exec := quotation trade
topexec := $(addprefix ../../$(notdir $(CURDIR))-,$(exec))

.PHONY : all clean

all :
	@echo $(sources)
	@echo $(execu)
	@echo $(topexecu)

#% : %.c 
#	echo $(topexec);
#	g++ $(headers) $< $(libmduserapi)  -o $@
#	cp $@ $(patsubst %,../../$(notdir $(CURDIR))-%,$@)

ifneq ($(MAKECMDGOALS),clean)
-include $(subst .c,.d,$(sources))
endif

%.d : %.c
	gcc -M $(headers) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

#quotation : quotation.c 
	#g++ $(headers) $< $(libmduserapi) -o $@
	#cp $@ $(patsubst %,../../$(notdir $(CURDIR))-%,$@)


clean :
	@$(RM) $(exec) $(topexec) *.d



