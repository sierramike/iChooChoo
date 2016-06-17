COMP=g++
EXEC=iChooChoo
LIBS=-lpthread
LOGTYPE=screen
OPTIONS=$(LIBS)
# -std=c99

# SRC=$(wildcard *.c)
SRC=main.c daemon.c server.c crc16.c i2c.c biccp.c modconf.c log_$(LOGTYPE).c
OBJ=$(SRC:.c=.o)

BININSTALLDIR=/usr/local/bin
WWWINSTALLDIR=/var/www/html

all: $(EXEC)

iChooChoo: $(OBJ)
	$(COMP) -o $@ $^ $(OPTIONS)

daemon.h: settings.h

main.o: main.c main.h header.h i2c.h modconf.h daemon.h

daemon.o: daemon.c daemon.h modconf.h

server.o: server.c server.h daemon.h

crc16.o: crc16.c crc16.h header.h

i2c.o: i2c.c i2c.h header.h

biccp.o: biccp.c biccp.h i2c.h crc16.h header.h

modconf.o: modconf.c modconf.h biccp.h log.h

log_screen.o: log_screen.c log.h

%.o: %.c
	$(COMP) -o $@ -c $< $(OPTIONS)

.PHONY: clean mrproper

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

install: iChooChoo
ifneq ("$(wildcard $(BININSTALLDIR)/iChooChoo)", "")
	$(error Application is already installed. Please run 'make uninstall' before.)
else
	cp iChooChoo $(BININSTALLDIR)
	chown root:root $(BININSTALLDIR)/iChooChoo
	chmod 755 $(BININSTALLDIR)/iChooChoo
	mkdir $(WWWINSTALLDIR)/iChooChoo
	cp -r www/* $(WWWINSTALLDIR)/iChooChoo/
endif

uninstall:
	rm $(BININSTALLDIR)/iChooChoo
	rm -r $(WWWINSTALLDIR)/iChooChoo
