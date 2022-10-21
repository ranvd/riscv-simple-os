SECTION = \
	./lecture/*

all:
	for dir in $(SECTION); do $(MAKE) -C $$dir; done

.PHONY : clean
clean:
	for dir in $(SECTION); do $(MAKE) clean -C $$dir; done