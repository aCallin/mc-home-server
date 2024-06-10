#!/bin/bash

screen -dmS minecraft bash -c 'cd /usr/local/sbin/mc-home-server/minecraft; java -Xmx4G -Xms4G -jar server.jar nogui'
screen -dmS heartbeat bash -c 'cd /usr/local/sbin/mc-home-server/heartbeat; ./heartbeat.out'
screen -dmS restart bash -c 'cd /usr/local/sbin/mc-home-server/restart; ./restart.out'
