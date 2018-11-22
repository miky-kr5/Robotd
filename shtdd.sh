#! /bin/sh
### BEGIN INIT INFO
# Provides: shtd
# Required-Start: 
# Required-Stop: 
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: Shutdown daemon.
### END INIT INFO

start() {
    if pidof shtdd > /dev/null
    then
	echo "shtdd already started."
	return
    else
	echo "Starting shtdd"
	shtdd
	echo "Done."
    fi
}

stop() {
    if pidof shtdd > /dev/null
    then
	echo "Stopping shtdd"
        kill -s INT `pidof shtdd`
        echo "Done."
    else
	echo "shtdd already stopped."
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
	if pidof shtdd > /dev/null
	then
	    echo "shtdd is running."
	else
	    echo "shtdd is down."
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
