COMP:=g++
EXEC:=iChooChoo
LIBS:=-lpthread
LOGTYPE:=screen
OPTIONS:=$(LIBS) -std=c++11 -MMD -MP
#OPTIONS:=$(LIBS) -DVERBOSEDEBUG
# -std=c99

SRC:=$(wildcard *.c)
SRC:=$(filter-out log_*.c, $(SRC))
SRC:=$(SRC) $(wildcard *.cpp)
SRC:=$(SRC) log_$(LOGTYPE).c

OBJ:=$(SRC:.cpp=.o)
OBJ:=$(OBJ:.c=.o)

DPD:=$(OBJ:.o=.d)

## SRC=$(wildcard *.c)
#SRCPP=$(wildcard *.cpp)
##SRC=main.c daemon.c server.c crc16.c i2c.c biccp.c modconf.c log_$(LOGTYPE).c ConfManager.cpp cConfElement.cpp cConfPosition.cpp cConfPosition.cpp cConfModule.cpp cConfSection.cpp
#SRC=main.c daemon.c server.c crc16.c i2c.c biccp.c modconf.c log_$(LOGTYPE).c $(SRCPP)
#OBJ0=$(SRC:.cpp=.o)
#OBJ=$(OBJ0:.c=.o)
#DPD=$(OBJ:.o=.d)

BININSTALLDIR:=/usr/local/bin
WWWINSTALLDIR:=/var/www/html
CNFINSTALLDIR:=/var/local/iChooChoo

all: $(EXEC)

iChooChoo: $(OBJ)
	$(COMP) -o $@ $^ $(OPTIONS)

-include $(DPD)

%.o: %.c
	$(COMP) -o $@ -c $< $(OPTIONS)

%.o: %.cpp
	$(COMP) -o $@ -c $< $(OPTIONS)

.PHONY: clean mrproper

clean:
	rm -rf *.o
	rm -rf *.d

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
	cp ./initd/iChooChoo /etc/init.d/iChooChoo
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

reinstallwww:
	-rm -r $(WWWINSTALLDIR)/iChooChoo
	mkdir $(WWWINSTALLDIR)/iChooChoo
	cp -r www/* $(WWWINSTALLDIR)/iChooChoo/
