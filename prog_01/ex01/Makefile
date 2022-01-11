MCU=atmega328p
CC=avr-gcc
OBJCOPY=avr-objcopy
F_CPU=16000000
TARGET=main
SRCS=main.c

all: hex flash

hex:
	${CC} -mmcu=${MCU} -D F_CPU=${F_CPU} -o ${TARGET}.bin ${SRCS}
	${OBJCOPY} -O ihex ${TARGET}.bin ${TARGET}.hex

flash:
	avrdude -p ${MCU} -c arduino -U flash:w:${TARGET}.hex:i -F -P /dev/ttyUSB*

clean:
	rm -f ${TARGET}.bin ${TARGET}.hex