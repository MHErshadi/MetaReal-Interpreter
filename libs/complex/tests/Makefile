CC = gcc
CFLAGS = -Wall -std=c99

SDIR = .
ODIR = .

SRCS = $(shell find $(SDIR) -name "*.c")
OUTS = $(patsubst $(SDIR)/%.c, $(ODIR)/%.exe, $(SRCS))

LIBS = ../../libcomplex.a ../../libfloat.a ../../libint.a -lgmp -lmpfr -lmpc

make: $(OUTS)

remake: clean $(OUTS)

$(ODIR)/%.exe: $(SDIR)/%.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@ -I ../..

clean:
	@rm -f $(shell find $(ODIR) -name "*.exe")

.PHONY: make remake clean
