CC = x86_64-w64-mingw32-gcc
CFLAGS ?= -O2
LDFLAGS ?= -lcomctl32 -lgdi32 -luser32

all: syscolor_demo.exe

syscolor_demo.exe: syscolor_demo.c
	$(CC) $(CFLAGS) -municode $< -o $@ $(LDFLAGS)

clean:
	rm -f syscolor_demo.exe

.PHONY: all clean
