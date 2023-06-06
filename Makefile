CC=gcc
CFLAGS=-Wall -g
LDLIBS=-lX11

PROGRAM=quickpick

$(PROGRAM): $(PROGRAM).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(PROGRAM).o: $(PROGRAM).c
	$(CC) $(CFLAGS) -c $< -o $@

uninstall:
	rm -f $(PROGRAM) *.o
	rm -f /usr/local/bin/$(PROGRAM)

install:
	cp $(PROGRAM) /usr/local/bin/
