CFLAGS=-g -Wall -Wextra -Werror -pedantic

.PHONY: clean default

default: papo peco

%: %.c bop_util.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f papo
	rm -f peco
	rm -rf *.dSYM
