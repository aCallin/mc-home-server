#!/bin/bash

# Assert root privileges
if [ `id -u` -ne 0 ]
then
	echo "Please run with root privileges"
	exit
fi

# Shut down restart screen
RESTART_SCREEN_NAME=restart
echo "Shutting down $RESTART_SCREEN_NAME screen"
screen -S $RESTART_SCREEN_NAME -p 0 -X stuff "^C" > /dev/null
screen -S $RESTART_SCREEN_NAME -Q select . > /dev/null
if [ $? -eq 0 ]
then
	echo "$RESTART_SCREEN_NAME screen did not shut down"
fi

# Shut down heartbeat screen
HEARTBEAT_SCREEN_NAME=heartbeat
echo "Shutting down $HEARTBEAT_SCREEN_NAME screen"
screen -S $HEARTBEAT_SCREEN_NAME -p 0 -X stuff "^C" > /dev/null
screen -S $HEARTBEAT_SCREEN_NAME -Q select . > /dev/null
if [ $? -eq 0 ]
then
	echo "$HEARTBEAT_SCREEN_NAME screen did not shut down"
fi

# Shut down minecraft screen
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
/usr/local/sbin/mc-home-server/scripts/backup.sh

# Done
echo "Done"
