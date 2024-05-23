RISCV_PREFIX = ../../riscv64-gcc/bin/riscv64-unknown-elf-

AR    = $(RISCV_PREFIX)ar
CC    = $(RISCV_PREFIX)gcc
STRIP = $(RISCV_PREFIX)strip

NM    = $(RISCV_PREFIX)nm
OBJDUMP = $(RISCV_PREFIX)objdump

SRC_DIR = ./src
INC_DIR = -I../educore-v/include -I./include
OUT_DIR = ./bin

CFLAGS  = -march=rv32i -mabi=ilp32 $(INC_DIR) -Os

_OBJ  = croutine event_groups list queue stream_buffer tasks timers
_OBJ += portable/port portable/portASM heap/heap_3 
OBJ = $(patsubst %,$(OUT_DIR)/%.o,$(_OBJ))


all: libfreertos.a


$(OUT_DIR):
	- mkdir $@
	- mkdir $@\portable
	- mkdir $@\heap

$(OUT_DIR)/%.o: $(SRC_DIR)/%.S $(OUT_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c $(OUT_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

libfreertos.a: $(OBJ)
	$(AR) -crs lib/$@ $^

clean:
	rm -rf $(OUT_DIR) lib/libfreertos.a


.PHONY: clean
