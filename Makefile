examples := examples
API := API

examples_sources := $(wildcard $(examples)/*/*.c)
examples_dirs := $(sort $(dir $(examples_sources)))
examples_dirsname := $(patsubst $(examples)/%/,%,$(examples_dirs))

.PHONY : all clean $(examples) $(examples_dirsname) 

all : $(examples) 

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
