TARGET = fuckread fuckwrite
ARMCC = arm-none-linux-gnueabi-gcc
CC = gcc

all:$(TARGET)

fuckread: fuckread.c
	$(ARMCC) -o $@ $< -Wall

fuckwrite: fuckwrite.c
	$(CC) -o $@ $< -Wall

clean:
	rm -rf $(TARGET)
