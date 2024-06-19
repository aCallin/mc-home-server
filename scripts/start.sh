#!/bin/bash

# Assert root privileges
if [ `id -u` -ne 0 ]
then
	echo "Please run with root privileges"
	exit
fi

# Start each screen in order: minecraft, heartbeat, restart
screen -dmS minecraft bash -c 'cd /usr/local/sbin/mc-home-server/minecraft; java -Xmx4G -Xms4G -jar server.jar nogui'
screen -dmS heartbeat bash -c 'cd /usr/local/sbin/mc-home-server/heartbeat; ./heartbeat.out'
screen -dmS restart bash -c 'cd /usr/local/sbin/mc-home-server/scripts; ./auto-restart.sh'
