#! /usr/bin/env python

import mraa
import syslog

GREEN_LED = 2
RED_LED = 4

syslog.openlog("robot.py", syslog.LOG_PID | syslog.LOG_NDELAY, syslog.LOG_USER)
syslog.syslog(syslog.LOG_NOTICE, "Robot.py started.")

try:
    red = mraa.Gpio(RED_LED)
    red.dir(mraa.DIR_OUT)
    green = mraa.Gpio(GREEN_LED)
    green.dir(mraa.DIR_OUT)

    while True:
        red.write(1)
        green.write(1)

except KeyboardInterrupt as e:
    syslog.syslog(syslog.LOG_ERR, str(e))

finally:
    syslog.syslog(syslog.LOG_NOTICE, "Shutting down LEDs.")
    red.write(0)
    green.write(0)
    syslog.syslog(syslog.LOG_NOTICE, "Robot.py finished.")
    syslog.closelog()
