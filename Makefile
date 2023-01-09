CC = gcc
CFLAGS = -O3 -Wall -std=c99 -Wno-unused-variable

SDIR = srcs
ODIR = objs
HDIR = heads
LDIR = libs
TDIR = tests

SRCS = $(shell find $(SDIR) -name "*.c")
OBJS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS))
LIBS = $(shell find $(LDIR) -name "*.a")
TSTS = $(shell find $(TDIR) -name "*.mr")
TRES = $(patsubst $(TDIR)/%.mr, $(TDIR)/%.x, $(TSTS))
OUT = metareal.exe

make: $(OUT)

remake: clean $(OUT)

all: $(OUT) test

rall: clean $(OUT) test

test: $(TRES)

$(OUT): $(OBJS)
	$(CC) -o $(OUT) $(OBJS) $(LIBS) -lgmp -lmpfr -lmpc

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $< -I $(HDIR) -I $(LDIR)

$(TDIR)/%.x: $(TDIR)/%.mr
	metareal $<

clean:
	@rm -f $(shell find $(ODIR) -name "*.o")
	@rm -f $(OUT)

.PHONY: make remake clean
