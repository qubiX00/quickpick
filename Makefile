CC=gcc
CFLAGS=-Wall -g
LDLIBS=-lX11

PROGRAM=myprogram

$(PROGRAM): $(PROGRAM).o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(PROGRAM).o: $(PROGRAM).c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PROGRAM) *.o
