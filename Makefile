TARGET = fuckread fuckwrite
#ARMCC = arm-none-linux-gnueabi-gcc
CC = gcc

all:$(TARGET)

fuckread: fuckread.c
	$(CC) -o $@ $< -Wall -g

fuckwrite: fuckwrite.c
	$(CC) -o $@ $< -Wall

clean:
	rm -rf $(TARGET)

up: fuckread
	adb push fuckread /system
