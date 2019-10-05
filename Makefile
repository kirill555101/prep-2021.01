TARGET = main.out
HDRS_DIR = project/include

SRCS = project/src/main.c

.PHONY: all clean

all: $(SRCS)
	$(CC) -Wall -Wextra -Werror -o $(TARGET) $(CFLAGS) $(SRCS)

clean:
	rm -rf $(TARGET)
