CC = gcc
CFLAGS = -Wall

SDIR = .
ODIR = .

SRCS = $(shell find $(SDIR) -name "*.c")
OUTS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.exe, $(SRCS))

LIB = ../../libmemory.a

make: $(OUTS)

remake: clean $(OUTS)

$(ODIR)/%.exe: $(SDIR)/%.c
	$(CC) $< $(LIB) -o $@ -I ..

clean:
	@rm -f $(shell find $(ODIR) -name "*.exe")

.PHONY: make remake clean
