CC=gcc

ROOT=..

UNITY=$(ROOT)/../unity

INC_DIR=-I$(ROOT)/../../.. -I$(UNITY) -I$(ROOT)

TEST_SRC=test_convert.c # The test_X.c can be hidden away and only the object file is needed to compile
TEST_OBJ=test_convert.o

SRC_FILES=$(ROOT)/convert.c $(UNITY)/unity.c

TARGET=test

CFLAGS=-ffunction-sections \
	   -fdata-sections \
	   -std=c99 \
	   -Wall \
	   -pedantic \
	   -Os

.PHONY=$(TEST_OBJ) clean

# Rule to build the test and execute it immediately (only if obj is built beforehand)
create: $(SRC_FILES) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(INC_DIR) -o $(TARGET) $(SRC_FILES) $(TEST_OBJ)
	- ./$(TARGET)

clean:
	@rm $(TARGET)
