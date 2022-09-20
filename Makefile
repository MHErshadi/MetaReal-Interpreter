CC = gcc

SDIR = srcs
ODIR = objs
HDIR = heads
LDIR = libs

SRCS = $(shell find $(SDIR) -name "*.c")
OBJS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS))
LIBS = $(wildcard *.a)
OUT = metareal.exe

make: $(OUT)

remake: clean $(OUT)

$(OUT): $(OBJS)
	$(CC) -o $(OUT) $(OBJS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< -I $(HDIR) -I $(LDIR)

clean:
	@rm -f $(shell find $(ODIR) -name "*.o")
	@rm -f $(OUT)

.PHONY: make remake clean
