SUBDIRS = graph_generator input_generator
.PHONY: $(SUBDIRS) clean

all: $(SUBDIRS)
$(SUBDIRS): 
	$(MAKE) -C $@

install:
	mkdir -p ../bin
	cp graph_generator/*_gen ../bin
	cp input_generator/*_gen ../bin


clean:
	cd graph_generator && $(MAKE) clean
	cd input_generator && $(MAKE) clean
	rm -rf ../bin
	rm -rf ../lib/*.a 
