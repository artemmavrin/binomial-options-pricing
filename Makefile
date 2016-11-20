CFLAGS=-g -Wall -Wextra -Werror -pedantic

.PHONY: clean default

default: papo peco

papo: papo.c
	${CC} ${CFLAGS} -o $@ $^

peco: peco.c
	${CC} ${CFLAGS} -o $@ $^

clean:
	rm -f papo
	rm -f peco
	rm -rf *.dSYM
