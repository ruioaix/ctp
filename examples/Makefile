sources := $(wildcard */*.c)
dirs := $(sort $(dir $(sources)))
dirsname := $(patsubst %/,%,$(dirs))

.PHONY : all clean $(dirsname)

all : $(dirsname)

$(dirsname) :
	@$(MAKE) -C $@ -f ../Makefile.sub.mk all

clean :
	@for d in $(dirsname); \
		do \
		$(MAKE) -C $$d -f ../Makefile.sub.mk clean;\
		done
