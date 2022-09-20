CC = gcc

SDIR = srcs
ODIR = objs

SRCS = $(shell find $(SDIR) -name "*.c")
OBJS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRCS))
HEAD = memory.h
LIB = ../libmemory.a

make: $(LIB)
	@cp $(HEAD) ../$(HEAD)

remake: clean $(LIB)
	@cp $(HEAD) ../$(HEAD)

clean:
	@rm -f $(shell find $(ODIR) -name "*.o")
	@rm -f $(LIB)
	@rm -f ../$(HEAD)

$(LIB): $(OBJS)
	ar -r $(LIB) $(OBJS)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< -I . -I ..

.PHONY: make remake clean
