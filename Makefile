TARGET = robotd shutdownd
OBJECT = robotd.o shutdownd.o
CFLAGS = -std=c11 -Wall -g -O0 -D _GNU_SOURCE
LDLIBS = -lmraa

.PHONY: all
all: robotd shutdownd

robotd: LDLIBS += `pkg-config --libs python`
robotd: robotd.o

robotd.o: CFLAGS += `pkg-config --cflags python`
robotd.o: robotd.c

shutdownd: shutdownd.o

shutdownd.o: shutdownd.c

install: robotd shutdownd init.sh shtdd.sh robot.service shutdown.service
	cp robotd /usr/bin/robotd
	cp shutdownd /usr/bin/shtdd
	cp init.sh /etc/init.d/robot
	cp shtdd.sh /etc/init.d/shtd
	cp robot.service /lib/systemd/system
	cp shutdown.service /lib/systemd/system
	chmod a+x /etc/init.d/robot
	chmod a+x /etc/init.d/shtd
	systemctl enable robot.service
	systemctl enable shutdown.service

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJECT)
