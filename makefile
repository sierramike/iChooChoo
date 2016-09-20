COMP=g++
EXEC=iChooChoo
LIBS=-lpthread
LOGTYPE=screen
OPTIONS=$(LIBS) -std=c++11
#OPTIONS=$(LIBS) -DVERBOSEDEBUG
# -std=c99

# SRC=$(wildcard *.c)
SRCPP=$(wildcard *.cpp)
#SRC=main.c daemon.c server.c crc16.c i2c.c biccp.c modconf.c log_$(LOGTYPE).c ConfManager.cpp cConfElement.cpp cConfPosition.cpp cConfPosition.cpp cConfModule.cpp cConfSection.cpp
SRC=main.c daemon.c server.c crc16.c i2c.c biccp.c modconf.c log_$(LOGTYPE).c $(SRCPP)
OBJ0=$(SRC:.cpp=.o)
OBJ=$(OBJ0:.c=.o)

BININSTALLDIR=/usr/local/bin
WWWINSTALLDIR=/var/www/html
CNFINSTALLDIR=/var/local/iChooChoo

all: $(EXEC)

iChooChoo: $(OBJ)
	$(COMP) -o $@ $^ $(OPTIONS)

daemon.h: settings.h

main.o: main.c main.h header.h i2c.h modconf.h daemon.h settings.h ConfManager.hpp

daemon.o: daemon.c daemon.h modconf.h settings.h

server.o: server.c server.h daemon.h

crc16.o: crc16.c crc16.h header.h

i2c.o: i2c.c i2c.h header.h

biccp.o: biccp.c biccp.h i2c.h crc16.h header.h

modconf.o: modconf.c modconf.h biccp.h log.h

log_screen.o: log_screen.c log.h

ConfManager.o : ConfManager.hpp cConfPosition.hpp cConfModule.hpp cConfSection.hpp cConfSwitch.hpp cConfRelay.hpp cConfSensor.hpp xstdstring.hpp

cConfElement.o : cConfElement.cpp cConfElement.hpp

cConfModuleAttachedElement.o : cConfModuleAttachedElement.cpp cConfModuleAttachedElement.hpp cConfElement.hpp

cConfPosition.o : cConfPosition.cpp cConfPosition.hpp cConfElement.hpp

cConfModule.o : cConfModule.cpp cConfModule.hpp cConfElement.hpp

cConfModuleTraction.o : cConfModuleTraction.cpp cConfModuleTraction.hpp cConfModule.hpp cConfElement.hpp biccp.h

cConfModuleGenPurp.o : cConfModuleGenPurp.cpp cConfModuleGenPurp.hpp cConfModule.hpp cConfElement.hpp biccp.h

cConfModuleLighting.o : cConfModuleLighting.cpp cConfModuleLighting.hpp cConfModule.hpp cConfElement.hpp biccp.h

cConfSection.o : cConfSection.cpp cConfSection.hpp cConfModule.hpp cConfModuleAttachedElement.hpp cConfElement.hpp

cConfSwitch.o : cConfSwitch.cpp cConfSwitch.hpp cConfModule.hpp cConfModuleAttachedElement.hpp cConfElement.hpp

cConfRelay.o : cConfRelay.cpp cConfRelay.hpp cConfModule.hpp cConfModuleAttachedElement.hpp cConfElement.hpp

cConfSensor.o : cConfSensor.cpp cConfSensor.hpp cConfModule.hpp cConfModuleAttachedElement.hpp cConfElement.hpp

%.o: %.c
	$(COMP) -o $@ -c $< $(OPTIONS)

%.o: %.cpp
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
	groupadd ichoochoo
	useradd -g ichoochoo ichoochoo
	usermod --groups spi,i2c,gpio,ichoochoo ichoochoo
	cp ./init.d/iChooChoo /etc/init.d/iChooChoo
	chmod 0755 /etc/init.d/iChooChoo
	systemctl daemon-reload
	update-rc.d iChooChoo defaults
	/etc/init.d/iChooChoo start
endif

uninstall:
	-/etc/init.d/iChooChoo force-stop
	update-rc.d -f iChooChoo remove
	-rm /etc/init.d/iChooChoo
	systemctl daemon-reload
	-userdel ichoochoo
	-groupdel ichoochoo
	-rm $(BININSTALLDIR)/iChooChoo
	-rm -r $(WWWINSTALLDIR)/iChooChoo

installconfig:
	cp iChooChoo.conf $(CNFINSTALLDIR)
	chown ichoochoo:ichoochoo $(CNFINSTALLDIR)/iChooChoo.conf
	chmod 644 $(CNFINSTALLDIR)/iChooChoo.conf
