SUBDIRS = graph_generator input_generator
.PHONY: $(SUBDIRS) clean

all: $(SUBDIRS)
$(SUBDIRS): 
	$(MAKE) -C $@

clean:
	cd graph_generator && $(MAKE) clean
	cd input_generator && $(MAKE) clean
