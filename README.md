# mc-home-server (W.I.P.)
Tools for hosting a Minecraft server at home.

## About
At its core, hosting a Minecraft server is relatively easy. However, more work is required if the server is to be set up with automatic restarts and backups, as well as a site to query the server's status. All of this can be handled with a few shell scripts, some C programming, and a systemd service, and results in a hands-off hosting experience.  

Tools overview:
- _Heartbeat_: a heartbeat program that allows clients to query the server's status over the web. Serves information such as CPU and memory usage, uptime, and date of query. Can be easily modified to add or remove information.
- _Restart_: automatically shuts down the Minecraft server and tools and restarts the system. Sends warning messages to the Minecraft server at set periods of time (60 minutes until restart, 30 minutes, etc.). Restart and warning times can be easily modified.
- _Backup_: the latest backup is stored for each day when the server is restarted or manually shut down. How many backups are kept around can be easily modified.
- A service that can be enabled to automatically start the Minecraft server and tools when the system boots up. The server can be left on its own at this point.

## Requirements
- A dedicated computer (Raspberry Pi, old latop, etc.)
- A Linux distribution (headless is recommended)
- Some packages: `sudo`, `gcc`, `make`, `screen`, `openjdk-[latest version]-jdk`
- ssh is not required but highly recommended

## Getting Started
Note that `sudo` may be required for most of the following.  

Setup:
1. Clone the repo into `/usr/local/sbin/` so that you have `/usr/local/sbin/mc-home-server/`
2. Navigate to the heartbeat folder and type `make` to build the program
3. Navigate to the minecraft folder and download server.jar from the [Minecraft website](https://www.minecraft.net/en-us/download/server)
4. Run the server for the first time with `java -Xmx1024M -Xms1024M -jar server.jar nogui` and accept the EULA
5. Move mc-home-server.service into `/etc/systemd/system/` and enable the service with `systemctl enable mc-home-server`

From this point on, the Minecraft server and tools will run automatically in the background whenever the system boots. You can also use `systemctl [start / stop / status] mc-home-server` to manually start or stop the server and tools, or check its status. Use the system's IP address to join the Minecraft server or to check the output from the heartbeat program.

## Appendix
