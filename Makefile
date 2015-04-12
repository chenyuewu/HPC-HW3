SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,%,$(SRCS))
CC = icc
CFLAGS = -openmp -O3

all: $(PROGS)

%: %.c
		$(CC) $(CFLAGS)  -o $@ $<

clean:
		rm -rf $(PROGS)
