SRC = $(wildcard *.c)
BIN = $(patsubst %.c,%,$(SRC))

CFLAGS += -Werror -Wall -Wextra -g `pkg-config fuse --cflags --libs`
#CFLAGS += --std=c99

all: $(BIN)

clean:
	rm -f $(BIN)
	rm -f *.o
	rm -f *~

indent:
	indent -linux -i4 -nut -ts2 *.c

.PHONY: all clean indent
