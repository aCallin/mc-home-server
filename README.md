# mc-home-server (W.I.P.)
Tools for hosting a Minecraft server at home.

## About
At its core, hosting a Minecraft server is relatively easy. However, more work is required if the server is to be set up with automatic restarts and backups, as well as a site to query the server's status. Luckily, all of this can be handled with a few shell scripts, some C programming, and a systemd service.

## Requirements
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
