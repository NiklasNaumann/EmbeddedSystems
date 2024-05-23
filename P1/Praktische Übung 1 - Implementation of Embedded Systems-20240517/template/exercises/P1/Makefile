RISCV_PREFIX = ../../riscv64-gcc/bin/riscv64-unknown-elf-

AR    = $(RISCV_PREFIX)ar
CC    = $(RISCV_PREFIX)gcc
STRIP = $(RISCV_PREFIX)strip

NM    = $(RISCV_PREFIX)nm
OBJDUMP = $(RISCV_PREFIX)objdump

SRC_DIR = ./src
INC_DIR = -I../../libs/educore-v/include -I./src
LIB_DIR = -L../../libs/educore-v/lib
OUT_DIR = ./obj

CFLAGS  = -march=rv32i -mabi=ilp32 -Os

_OBJ = main
OBJ = $(patsubst %,$(OUT_DIR)/%.o,$(_OBJ))


all: program

$(OUT_DIR): 
	- mkdir $@

$(OUT_DIR)/%.o: $(OUT_DIR)

$(OUT_DIR)/%.o: $(SRC_DIR)/%.S $(OUT_DIR)
	$(CC) -c -o $@ $< $(CFLAGS) $(INC_DIR)
$(OUT_DIR)/%.o: $(SRC_DIR)/%.c $(OUT_DIR)
	$(CC) -c -o $@ $< $(CFLAGS) $(INC_DIR)

%.bin: $(OBJ)
	$(CC) -o $@ $^ $(LIB_DIR) $(CFLAGS) \
		-nostartfiles \
		-lc -leducore-v \
		--specs=nano.specs \
		-Wl,--gc-sections \
		-Wl,--section-start=.init=0x0

%.hex: %.bin
	$(STRIP) $< -O binary -o $@

%.mem: %.hex
	- xxd -g 4 -c 4 -e $< > $@

disassemble: program.bin
	$(OBJDUMP) -S $<

program: program.hex

clean:
	rm -rf $(OUT_DIR) program.*


.PHONY: clean program
