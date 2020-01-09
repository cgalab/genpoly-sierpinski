make_sierpinski: make_sierpinski.c
	$(CC) -O2 -Wall -o "$@" $<

clean:
	rm -f make_sierpinski

.PHONY: clean
