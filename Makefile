TARGET = fuckread fuckwrite
ARMCC = arm-none-linux-gnueabi-gcc
CC = gcc

all:$(TARGET)

fuckread: fuckread.c
	$(ARMCC) -o $@ $< -Wall -static -g

fuckwrite: fuckwrite.c
	$(CC) -o $@ $< -Wall

clean:
	rm -rf $(TARGET)

up: fuckread
	adb push fuckread /system
