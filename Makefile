CFILES   = $(wildcard *.c)
OBJFILES = $(CFILES:.c=.o)
OUT      = main

CC      = gcc
CFLAGS  = -Wall -I ./
LDLIBS  = -lpaho-mqtt3cs 


$(OUT): $(OBJFILES)

.PHONY: clean
clean:
	rm -f $(OBJFILES) $(OUT)
