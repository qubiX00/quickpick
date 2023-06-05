CC=gcc
CFLAGS=-Wall -g
LDLIBS=-lX11

PROGRAM=quickpick

$(PROGRAM): $(PROGRAM).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(PROGRAM).o: $(PROGRAM).c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PROGRAM) *.o

install:
	cp $(PROGRAM) /usr/local/bin/
