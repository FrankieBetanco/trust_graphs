SUBDIRS = graph_generator input_generator trust_graphs
.PHONY: $(SUBDIRS) clean

all: $(SUBDIRS)
$(SUBDIRS): 
	$(MAKE) -C $@

install:
	mkdir ../bin
	cp graph_generator/*_gen ../bin/
	cp input_generator/*_gen ../bin/


clean:
	cd graph_generator && $(MAKE) clean
	cd input_generator && $(MAKE) clean
	cd trust_graphs && $(MAKE) clean
	rm -rf ../bin ../lib
