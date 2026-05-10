.PHONY: clean dist

dist: clean
	tar -hzcf "$(CURDIR).tar.gz" hcompress01/* unhcompress01/* makefile

clean:
	$(MAKE) -C hcompress01 clean
	$(MAKE) -C unhcompress01 clean
