CC=gcc

SRC_FILE=check_linearity.c

TARGET=test

CFLAGS=-ffunction-sections \
	   -fdata-sections \
	   -std=c99 \
	   -Wall \
	   -pedantic \
	   -Os

.PHONY=clean

# Rule to build the test and execute it immediately
create: $(SRC_FILE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC_FILE)
	- ./$(TARGET)

clean:
	@rm $(TARGET)
