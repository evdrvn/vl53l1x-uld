CC = $(CROSS_COMPILE)gcc
AR = $(CROSS_COMPILE)ar
RM = rm

CFLAGS = -O0 -g -Wall -c

OUTPUT_DIR = bin
OBJ_DIR = obj
API_DIR = STSW-IMG009
SRC_DIR = src

LIB_NAME = vl53l1x-uld
TARGET_LIB = $(OUTPUT_DIR)/lib$(LIB_NAME).a

INCLUDES = \
	-I$(API_DIR)/API/core \
	-I$(SRC_DIR)

LIB_SRCS = \
	$(API_DIR)/API/core/VL53L1X_calibration.c \
	$(API_DIR)/API/core/VL53L1X_api.c \
	$(SRC_DIR)/vl53l1_platform.c

LIB_OBJS  = $(LIB_SRCS:%.c=$(OBJ_DIR)/%.o)

EXAMPLE_SRC = $(SRC_DIR)/vl53l1_example.c
EXAMPLE_BIN = $(OUTPUT_DIR)/vl53l1_example

.PHONY: all
all: clean example

.PHONY: example 
example: ${TARGET_LIB} $(EXAMPLE_BIN)

.PHONY: lib 
lib: ${TARGET_LIB}

.PHONY: clean
clean:
	${RM} -rf ./$(OUTPUT_DIR)/*  ./$(OBJ_DIR)/*

$(TARGET_LIB): $(LIB_OBJS)
	mkdir -p $(dir $@)
	$(AR) -rcs $@ $^

$(OBJ_DIR)/%.o:%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

$(EXAMPLE_BIN): $(EXAMPLE_SRC)
	mkdir -p $(dir $@)
	$(CC) -L$(OUTPUT_DIR) $^ -l$(LIB_NAME) $(INCLUDES) -o $@

