SUBDIRS = graph_generator input_generator trust_graphs
.PHONY: $(SUBDIRS) clean

all: $(SUBDIRS)
$(SUBDIRS): 
	$(MAKE) -C $@

install:
	mkdir -p ../bin
	mkdir -p ../lib
	cp graph_generator/*_gen ../bin/
	mv graph_generator/*.a ../lib/
	cp input_generator/*_gen ../bin/
	mv input_generator/*.a ../lib/


clean:
	cd graph_generator && $(MAKE) clean
	cd input_generator && $(MAKE) clean
	cd trust_graphs && $(MAKE) clean
	rm -rf ../bin ../lib
