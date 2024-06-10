#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#define HEARTBEAT_SCREEN_NAME "heartbeat"
#define MINECRAFT_SCREEN_NAME "minecraft"
#define CHAR_BUFFER_SIZE 200
#define RESTART_TIME (6 * 60 * 60)
#define HANDLE_ERROR(message) { perror(message); return errno; }

int main(int argc, char *argv[]) {
    // Times at which to send warnings to the minecraft server about the server restarting
    const int warning_times[] = {
        60 * 60,
        30 * 60,
        15 * 60,
        5 * 60,
        1 * 60
    };
    const int warning_time_size = sizeof(warning_times) / sizeof(warning_times[0]);
    int has_warned[warning_time_size];
    memset(has_warned, 0, warning_time_size * sizeof(has_warned[0]));

    // For formatted strings
    char char_buffer[CHAR_BUFFER_SIZE];

    // Loop: get uptime, warn if uptime exceedes warning time, break if uptime exceedes restart time or sleep 5 seconds otherwise
    struct sysinfo system_info;
    while (1) {
        if (sysinfo(&system_info) == -1)
            HANDLE_ERROR("Failed to get system info")
        for (int i = 0; i < warning_time_size; i++) {
            if (has_warned[i] == 0 && warning_times[i] >= RESTART_TIME - system_info.uptime) {
                printf("Warning: the server will restart in %d minutes\n", warning_times[i] / 60);
                snprintf(char_buffer, CHAR_BUFFER_SIZE, "screen -S %s -p 0 -X stuff \"say Warning: the server will restart in %d minutes^M\"", MINECRAFT_SCREEN_NAME, warning_times[i] / 60);
                if (system(char_buffer) == -1)
                    HANDLE_ERROR("Warning bash command failed")
                has_warned[i] = 1;
            }
        }
        if (system_info.uptime >= RESTART_TIME)
            break;
        sleep(5);
    }

    // Shut down heartbeat screen
    printf("Shutting down heartbeat screen\n");
    snprintf(char_buffer, CHAR_BUFFER_SIZE, "screen -S %s -p 0 -X stuff \"^C\"", HEARTBEAT_SCREEN_NAME);
    if (system(char_buffer) == -1)
        HANDLE_ERROR("Shut down heartbeat screen command failed")

    // Shut down minecraft server screen
    printf("Shutting down minecraft server screen\n");
    snprintf(char_buffer, CHAR_BUFFER_SIZE, "screen -S %s -p 0 -X stuff \"stop^M\"", MINECRAFT_SCREEN_NAME);
    if (system(char_buffer) == -1)
        HANDLE_ERROR("Stop minecraft server command failed")
    snprintf(char_buffer, CHAR_BUFFER_SIZE, "screen -ls | grep -q \"%s\"", MINECRAFT_SCREEN_NAME);
    while (1) {
        const int grep_exit_code = system(char_buffer);
        if (grep_exit_code == -1)
            HANDLE_ERROR("grep command failed");
        if (grep_exit_code == 0)
            sleep(1);
        else
            break;
    }

    // Restart
    printf("Restarting\n");
    if (system("shutdown -r now") == -1)
        HANDLE_ERROR("Restart bash command failed")
}
