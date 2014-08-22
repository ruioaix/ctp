examples := examples
CTPAPI := CTPAPI

examples_sources := $(wildcard $(examples)/*/*.c)
examples_dirs := $(sort $(dir $(examples_sources)))
examples_dirsname := $(patsubst $(examples)/%/,%,$(examples_dirs))

.PHONY : all install clean $(examples) $(examples_dirsname) $(CTPAPI)

all : $(CTPAPI)
	
install : 
	@for d in $(CTPAPI); \
		do \
		$(MAKE) -C $$d install; \
		done

clean :
	@$(RM) *.con
	@for d in $(examples); \
		do \
		$(MAKE) -C $$d clean; \
		done

$(CTPAPI) $(examples) : 
	@$(MAKE) -C $@ all

$(examples_dirsname) : 
	@$(MAKE) -C $(examples) $@

