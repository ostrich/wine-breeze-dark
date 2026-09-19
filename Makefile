CC = x86_64-w64-mingw32-gcc
STRIP = x86_64-w64-mingw32-strip
CFLAGS ?= -O2 -Wall -Wextra
LDFLAGS ?= -mwindows
LDLIBS ?= -lcomctl32 -lgdi32 -luser32

all: syscolor-demo.exe

syscolor-demo.exe: syscolor-demo.c
	$(CC) $(CFLAGS) -municode $< -o $@ $(LDFLAGS) $(LDLIBS)
	$(STRIP) $@

clean:
	rm -f syscolor-demo.exe

.PHONY: all clean
