# mc-home-server (W.I.P.)
Tools for hosting a Minecraft server at home.

## About

## Requirements
- A Linux distribution (headless is recommended)
- Some packages: `sudo`, `gcc`, `make`, `screen`, `openjdk-[latest version]-jdk`
- ssh is not required but highly recommended

## Getting Started
Note that `sudo` may be required for most of the following.  

Setup:
1. Clone the repo into `/usr/local/sbin/` and `cd` into mc-home-server
2. `cd` into the heartbeat folder and type `make` to build the program
3. `cd` into the minecraft folder and download server.jar from the [Minecraft website](https://www.minecraft.net/en-us/download/server)
4. Run the server for the first time with `java -Xmx1024M -Xms1024M -jar minecraft_server.1.21.jar nogui` and accept the EULA
5. `mv` mc-home-server.service into `/etc/systemd/system/` and type `systemctl enable mc-home-server`

From this point on, the Minecraft server and tools will run automatically in the background when the system boots. You can also use `systemctl [start / stop / status] mc-home-server` to manually start or stop the server and tools, or check its status.
