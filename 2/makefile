.PHONY: clean dist

dist: clean
	tar -hzcf "$(CURDIR).tar.gz" bt/* bt_test/* makefile

clean:
	$(MAKE) -C bt_test clean
