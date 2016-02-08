EXEC=iChooChoo
LIBS=
LOGTYPE=screen
OPTIONS=$(LIBS) -std=c99

all: $(EXEC)

iChooChoo: main.o crc16.o i2c.o biccp.o modconf.o log_$(LOGTYPE).o
	gcc -o $(EXEC) main.o crc16.o i2c.o biccp.o modconf.o log_$(LOGTYPE).o $(OPTIONS)

main.o: main.c main.h header.h i2c.h modconf.h
	gcc -o main.o -c main.c $(OPTIONS)

crc16.o: crc16.c crc16.h header.h
	gcc -o crc16.o -c crc16.c $(OPTIONS)

i2c.o: i2c.c i2c.h header.h
	gcc -o i2c.o -c i2c.c $(OPTIONS)

biccp.o: biccp.c biccp.h i2c.h crc16.h header.h
	gcc -o biccp.o -c biccp.c $(OPTIONS)

modconf.o: modconf.c modconf.h biccp.h log.h
	gcc -o modconf.o -c modconf.c $(OPTIONS)

log_screen.o: log_screen.c log.h
	gcc -o log_screen.o -c log_screen.c $(OPTIONS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

install: iChooChoo
	cp iChooChoo /usr/local/bin
	chown root:root /usr/local/bin/iChooChoo
	chmod 755 /usr/local/bin/iChooChoo
