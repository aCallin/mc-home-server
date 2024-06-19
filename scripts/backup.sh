#!/bin/bash

# Assert root privileges
if [ `id -u` -ne 0 ]
then
	echo "Please run with root privileges"
	exit
fi

# Constants
NUM_BACKUPS=30
BACKUPS_FOLDER=/usr/local/sbin/mc-home-server/world-backups/

# Make the backup
mkdir -p $BACKUPS_FOLDER
NEW_BACKUP_NAME=`date | awk '{print $1"-"$2"-"$3"-"$4}'`
rm -rf $BACKUPS_FOLDER$NEW_BACKUP_NAME
cp -r /usr/local/sbin/mc-home-server/minecraft/world/ $BACKUPS_FOLDER$NEW_BACKUP_NAME
echo "Created new backup: $NEW_BACKUP_NAME"

# Remove old backups (> NUM_BACKUPS ago)
I=1
for BACKUP_NAME in `ls -t $BACKUPS_FOLDER`
do
    if [ $I -gt $NUM_BACKUPS ]
    then
        rm -rf $BACKUPS_FOLDER$BACKUP_NAME
        echo "Removed old backup: $BACKUP_NAME"
    fi
    ((I++))
done
