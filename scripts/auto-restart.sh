#!/bin/bash

# Assert root privileges
if [ `id -u` -ne 0 ]
then
	echo "Please run with root privileges"
	exit
fi

# Fill these out
RESTART_TIME=360 # Restart time in minutes
WARNING_TIMES=(60 30 15 10 5 1) # Warning times in minutes

# Prevent restart loop
if [ $RESTART_TIME -lt 10 ]
then
    echo "Restart time can't be less than 10 minutes"
    exit
fi

# Give summary of restart time
RESTART_TIME_HOURS=`expr $RESTART_TIME / 60`
RESTART_TIME_HOURS_SUB=`expr $RESTART_TIME_HOURS \* 60`
RESTART_TIME_MINUTES=`expr $RESTART_TIME - $RESTART_TIME_HOURS_SUB`
date
echo "Restart time: $RESTART_TIME minutes, or $RESTART_TIME_HOURS hour(s) and $RESTART_TIME_MINUTES minute(s) from uptime"

# Sleep until next warning time
M=m
for WARNING_TIME in ${WARNING_TIMES[@]}
do
    UPTIME=`awk '{print int($1/60)}' /proc/uptime`
    SLEEP_TIME=`expr $RESTART_TIME - $UPTIME - $WARNING_TIME`
    if [ $SLEEP_TIME -gt 0 ]
    then
        sleep $SLEEP_TIME$M
        TIMESTAMP=`date | awk '{print $5" "$6}'`
        echo "[$TIMESTAMP] $WARNING_TIME minute warning"
        screen -S minecraft -p 0 -X stuff "say Restarting in $WARNING_TIME minute(s)^M"
    fi
done
UPTIME=`awk '{print int($1/60)}' /proc/uptime`
SLEEP_TIME=`expr $RESTART_TIME - $UPTIME`
sleep $SLEEP_TIME$M

# Stop heartbeat and mc-home-server screens
HEARTBEAT_SCREEN_NAME=heartbeat
echo "Shutting down $HEARTBEAT_SCREEN_NAME screen"
screen -S $HEARTBEAT_SCREEN_NAME -p 0 -X stuff "^C" > /dev/null
screen -S $HEARTBEAT_SCREEN_NAME -Q select . > /dev/null
if [ $? -eq 0 ]
then
	echo "$HEARTBEAT_SCREEN_NAME screen did not shut down"
fi
MINECRAFT_SCREEN_NAME=minecraft
echo "Shutting down $MINECRAFT_SCREEN_NAME screen"
screen -S $MINECRAFT_SCREEN_NAME -p 0 -X stuff "stop^M" > /dev/null
screen -S $MINECRAFT_SCREEN_NAME -Q select . > /dev/null
while [ $? -eq 0 ]
do
	sleep 1
	echo "In progress"
	screen -S $MINECRAFT_SCREEN_NAME -Q select . > /dev/null
done

# Create / remove backups
./backup.sh

# Restart
echo "Restarting"
shutdown -r now