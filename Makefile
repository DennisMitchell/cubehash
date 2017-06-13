src = $(wildcard *.c)
obj = $(src:.c=.o)
dep = $(obj:.o=.d)

CFLAGS = -W{error,pedantic,all,extra} -std=c99 -O3 -march=native

cubehash: $(obj)
	$(CC) $(CFLAGS) -o $@ $^

-include $(dep)

%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	rm -f $(obj) cubehash

.PHONY: cleandep
cleandep:
	rm -f $(dep)

PREFIX = /usr/local

.PHONY: install
install: cubehash
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/cubehash
