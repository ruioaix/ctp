examples := examples
CTPAPI := CTPAPI
examples_dir := $(notdir $(patsubst %/,%,$(filter %/, $(wildcard $(examples)/*/))))
CTPAPI_dir := $(notdir $(patsubst %/,%,$(filter %/, $(wildcard $(CTPAPI)/*/))))

.PHONY : all install clean $(examples) $(CTPAPI) $(examples_dir) $(CTPAPI_dir)

all : $(CTPAPI) $(examples)
	
#only 
install : 
	@for d in $(CTPAPI); \
		do \
		$(MAKE) -C $$d install; \
		done

clean :
	@$(RM) *.con
	@for d in $(CTPAPI) $(examples); \
		do \
		$(MAKE) -C $$d clean; \
		done

$(CTPAPI) $(examples) : 
	@$(MAKE) -C $@ all

$(examples_dir) :
	@$(MAKE) -C $(examples) $@

$(CTPAPI_dir) :
	@$(MAKE) -C $(CTPAPI) $@
