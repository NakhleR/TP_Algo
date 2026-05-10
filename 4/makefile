.PHONY: clean dist

dist: clean
	tar -hzcf "$(CURDIR).tar.gz" heapsort/* pqueue/* pqueue_test/* holdall/* makefile

clean:
	$(MAKE) -C heapsort clean
	$(MAKE) -C pqueue_test clean
