examples := examples
CTPAPI := CTPAPI

examples_sources := $(wildcard $(examples)/*/*.c)
examples_dirs := $(sort $(dir $(examples_sources)))
examples_dirsname := $(patsubst $(examples)/%/,%,$(examples_dirs))

.PHONY : all clean $(examples) $(examples_dirsname) 

all : $(examples)

#@echo $(examples_sources)
#@echo $(examples_dirs)
#@echo $(examples_dirsname)

$(examples) : 
	@$(MAKE) -C $@ all

$(examples_dirsname) : 
	@$(MAKE) -C $(examples) $@

clean :
	@$(RM) *.con
	@for d in $(examples); \
		do \
		$(MAKE) -C $$d clean; \
		done
