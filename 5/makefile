.PHONY: clean dist

dist: clean
	tar -hzcf "$(CURDIR).tar.gz" avl/* bst_test/* op/* scale/* makefile

clean:
	$(MAKE) -C bst_test clean
