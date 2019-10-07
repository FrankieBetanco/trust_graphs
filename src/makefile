all: input_generator graph_generator

.PHONY: input_generator graph_generator clean

input_generator: 
	cd input_generator && $(MAKE)

graph_generator: 
	cd graph_generator && $(MAKE)

clean:
	cd graph_generator && $(MAKE) clean
	cd input_generator && $(MAKE) clean
