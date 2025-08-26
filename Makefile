CC = gcc
CFLAGS = -static
SRC = src
CMDS = $(SRC)/cmds
BUILD = build
BIN = $(BUILD)/bin
KERNEL = bzImage
DISK_IMG = os.img
MNT = /mnt/heavensos
DISK_SIZE = 64M

INIT_SRC := $(SRC)/init.c
SHELL_SRC := $(SRC)/shell.c
CMD_SRCS := $(wildcard $(CMDS)/*.c)
LIB_SRCS := $(wildcard $(SRC)/lib/*.c)

INIT_BIN := $(BUILD)/init
SHELL_BIN := $(BUILD)/shell
CMD_BINS := $(patsubst $(CMDS)/%.c, $(BIN)/%, $(CMD_SRCS))

all: run

build: $(INIT_BIN) $(SHELL_BIN) $(CMD_BINS)

$(INIT_BIN): $(INIT_SRC)
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $@ $<

$(SHELL_BIN): $(SHELL_SRC)
	mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $@ $<

$(BIN)/%: $(CMDS)/%.c $(LIB_SRCS)
	mkdir -p $(BIN)
	$(CC) $(CFLAGS) -o $@ $< $(LIB_SRCS)

fs: build
	@echo "[*] Erstelle leeres ext4-Image"
	rm -f $(DISK_IMG)
	dd if=/dev/zero of=$(DISK_IMG) bs=1M count=$(shell echo $(DISK_SIZE) | tr -d M)
	mkfs.ext4 $(DISK_IMG)

	@echo "[*] Mounten und Dateien kopieren..."
	sudo mkdir -p $(MNT)
	sudo mount -o loop $(DISK_IMG) $(MNT)

	sudo mkdir -p \
		$(MNT)/bin \
		$(MNT)/dev \
		$(MNT)/proc
	
	sudo cp $(INIT_BIN) $(MNT)/init
	sudo cp $(SHELL_BIN) $(MNT)/shell
	sudo cp -r $(BIN)/* $(MNT)/bin/

	sudo mknod -m 666 $(MNT)/dev/tty0 c 4 0

	sync
	sudo umount $(MNT)
	@echo "[V] Filesystem bereit in $(DISK_IMG)"

run: fs
	qemu-system-x86_64 \
		-kernel $(KERNEL) \
		-drive file=$(DISK_IMG),format=raw \
		-m 512M \
		-append "root=/dev/sda rw init=/init console=tty0 quiet" \
		-display sdl

clean:
	rm -rf $(BUILD) $(DISK_IMG)

