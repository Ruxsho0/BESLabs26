CC=gcc

ROOT=..

UNITY=$(ROOT)/../unity

INC_DIR=-I$(ROOT)/../../.. -I$(UNITY) -I$(ROOT)

TEST_SRC=test_sound_dac_piano.c # The test_X.c can be hidden away and only the object file is needed to compile
TEST_OBJ=test_sound_dac_piano.o

SRC_FILES=$(ROOT)/sound.c $(ROOT)/dac.c $(ROOT)/piano.c $(UNITY)/unity.c

TARGET=test

CFLAGS=-ffunction-sections \
	   -fdata-sections \
	   -std=c99 \
	   -Wall \
	   -pedantic \
	   -Os

DEFINES=-DNVIC_ST_RELOAD_R=nvic_st_reload \
		-DGPIO_PORTB_DATA_R=gpio_portb_data \
		-DGPIO_PORTE_DATA_R=gpio_porte_data

.PHONY=$(TEST_OBJ) clean

# Rule to build the test and execute it immediately (only if obj is built beforehand)
create: $(SRC_FILES) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(INC_DIR) -o $(TARGET) $(DEFINES) $(SRC_FILES) $(TEST_OBJ)
	- ./$(TARGET)

# Rule to build the object from the test source to be able to hide it away later
obj:
	$(CC) $(CFLAGS) $(INC_DIR) -c $(TEST_SRC) -o $(TEST_OBJ)

clean:
	@rm $(TARGET)
