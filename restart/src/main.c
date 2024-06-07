#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include <unistd.h>

#define RESTART_TIME (120 * 60) // TODO: change to more reasonable number
#define HANDLE_ERROR(message) { perror(message); return errno; }

int main(int argc, char *argv[]) {
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

    struct sysinfo system_info;
    while (1) {
        if (sysinfo(&system_info) == -1)
            HANDLE_ERROR("Failed to get system info")
        for (int i = 0; i < warning_time_size; i++) {
            if (has_warned[i] == 0 && warning_times[i] >= RESTART_TIME - system_info.uptime) {
                printf("Warning: the server will restart in %d minutes\n", warning_times[i] / 60);

                // TODO: message to minecraft server code here

                has_warned[i] = 1;
            }
        }
        if (system_info.uptime >= RESTART_TIME)
            break;
        sleep(5);
    }

    // TODO: shutdown minecraft server code here

    if (system("shutdown -r now") == -1)
        HANDLE_ERROR("Restart bash command failed")
}
