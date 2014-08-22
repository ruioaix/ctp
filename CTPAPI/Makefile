dirsname := $(patsubst %/,%,$(wildcard */))

.PHONY : all install clean $(dirsname) 

all : $(dirsname)

install : 
	@for d in $(dirsname); \
		do \
		$(MAKE) -C $$d install;\
		done

clean :
	@for d in $(dirsname); \
		do \
		$(MAKE) -C $$d clean;\
		done

$(dirsname) :
	@$(MAKE) -C $@ all
