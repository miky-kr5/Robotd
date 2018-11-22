#! /bin/bash
### BEGIN INIT INFO
# Provides: robotd
# Required-Start: 
# Required-Stop: 
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: Robot control daemon.
### END INIT INFO

start() {
    PID=`pidof robotd`
    if [ $? -eq 0 ]
    then
	echo "robotd already started."
	return
    else
	echo "Starting robotd"
	robotd
	echo "Done."
    fi
}

stop() {
    PID=`pidof weblabsd`
    if [ $? -eq 0 ]
    then
	echo "Stopping robotd"
        kill -s INT `pidof robotd`
        echo "Done."
    else
	echo "robotd already stopped."
    fi
}

case "$1" in
    start)
        start
        ;;
    stop)
        stop
        ;;
    status)
	PID=`pidof robotd`
	if [ $? -eq 0 ]
	then
	    echo "robotd is running."
	else
	    echo "robotd is down."
	fi
        ;;
    restart)
        stop
        start
        ;;
    *)
        echo "Usage:  {start|stop|restart|status}"
        exit 1
        ;;
esac
exit $?
