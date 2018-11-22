#! /bin/sh
### BEGIN INIT INFO
# Provides: robotd
# Required-Start: 
# Required-Stop: 
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: Robot control daemon.
### END INIT INFO

start() {
    if pidof robotd > /dev/null
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
    if pidof robotd > /dev/null
    then
	echo "Stopping robotd"
        kill -s INT `pidof robotd`
        echo "Done."
    else
	echo "robotd already stopped."
    fi
}

case "$1" in
    start|restart|force-reload)
        start
        ;;
    stop)
        stop
        ;;
    status)
	if pidof robotd > /dev/null
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
        echo "Usage:  {start|stop|restart|force-reload|status}"
        exit 1
        ;;
esac
