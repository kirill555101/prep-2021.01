TARGET_MAIN = main.out
HDRS_DIR_MAIN = project/include

SRCS_MAIN = \
       project/src/main.c \
       project/src/utils.c \
       project/src/work_with_file.c

TARGET_TEST = test.out
HDRS_DIR_TEST = project/include

SRCS_TEST = \
       project/src/test.c \
       project/src/tests.c \
       project/src/work_with_file.c \
       project/src/utils.c \
       project/src/equality_of_structures.c

.PHONY: all clean

all: main test

main: $(SRCS_MAIN)
	$(CC) -Wall -Wextra -Werror -I $(HDRS_DIR_MAIN) -o $(TARGET_MAIN) $(CFLAGS) $(SRCS_MAIN)

test: $(SRCS_TEST)
	$(CC) -Wall -Wextra -Werror -I $(HDRS_DIR_TEST) -o $(TARGET_TEST) $(CFLAGS) $(SRCS_TEST)

clean:
	rm -rf $(TARGET_MAIN)
	rm -rf $(TARGET_TEST)

