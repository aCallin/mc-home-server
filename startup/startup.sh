#!/bin/bash

screen -dmS mc-home-server-minecraft-server bash -c 'cd /usr/local/sbin/mc-home-server/minecraft; java -Xmx4G -Xms4G -jar server.jar nogui'
screen -dmS mc-home-server-heartbeat bash -c 'cd /usr/local/sbin/mc-home-server/heartbeat; ./heartbeat.out'
