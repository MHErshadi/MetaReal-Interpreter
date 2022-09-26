CC = gcc
CFLAGS =

SDIR = .
ODIR = .

SRCS = $(shell find $(SDIR) -name "*.c")
OUTS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.exe, $(SRCS))

LIB = ../../libmemory.a

make: $(OUTS)

remake: clean $(OUTS)

$(ODIR)/%.exe: $(SDIR)/%.c
	$(CC) $(CFLAGS) $< $(LIB) -o $@ -I ..

clean:
	@rm -f $(shell find $(ODIR) -name "*.exe")

.PHONY: make remake clean
