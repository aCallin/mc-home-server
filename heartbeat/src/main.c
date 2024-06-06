/**
 * Recommended usage:
 * Run on a device set up with a static IP
 * Bookmark http://ip:port on client devices
 * Refresh whenever you need
 * Close with Ctrl-C or other interrupt
*/

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define LISTEN_BACKLOG 10
#define STATUS_FILE_NAME "status.html"
#define HANDLE_ERROR(message) { perror(message); return errno; }
#define LOG(message) { printf("[%s] %s\n", timestamp(), message); }

char *timestamp() {
    static time_t calendar_time;
    static struct tm *calendar_time_components;
    static char timestamp_without_newline[25];
    calendar_time = time(NULL);
    calendar_time_components = localtime(&calendar_time);
    const char *const timestamp_with_newline = asctime(calendar_time_components);
    strncpy(timestamp_without_newline, timestamp_with_newline, sizeof(timestamp_without_newline));
    timestamp_without_newline[sizeof(timestamp_without_newline) - 1] = '\0';
    return timestamp_without_newline;
}

int main(int argc, char *argv[]) {
    // Server info
    const int port = htons(8080);
    const struct sockaddr_in address = {
        AF_INET,
        port,
        INADDR_ANY
    };
    
    // Create listening socket, bind it, listen for client
    const int listening_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_socket_fd == -1)
        HANDLE_ERROR("Failed to create listening socket")
    LOG("Listening socket created")
    if (bind(listening_socket_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
        HANDLE_ERROR("Failed to bind the listening socket")
    LOG("Bind successful")
    if (listen(listening_socket_fd, LISTEN_BACKLOG) == -1)
        HANDLE_ERROR("Failed to listen for connections")
    LOG("Listening for clients...")

    // Loop: accept client, update (via shell) + open + send + close status file, close client
    while (1) {
        const int connected_socket_fd = accept(listening_socket_fd, 0, 0);
        if (connected_socket_fd == -1)
            HANDLE_ERROR("Failed to accept client")
        LOG("Accepted client")
        // Unfortunately define constants (ex. STATUS_FILE_NAME) won't work here
        system("echo '<pre>' > temp");
        system("date >> temp");
        system("uptime -p >> temp");
        system("echo >> temp");
        system("free -h >> temp");
        system("echo >> temp");
        system("top -b -n 1 >> temp");
        system("echo '</pre>' >> temp");
        system("echo 'HTTP/1.0 200 OK' > status.html");
        system("echo 'Content-Type: text/html' >> status.html");
        system("echo \"Content-Length: $(wc -c < temp)\" >> status.html");
        system("echo >> status.html");
        system("cat temp >> status.html");
        const int status_file_fd = open(STATUS_FILE_NAME, O_RDONLY);
        if (status_file_fd == -1)
            HANDLE_ERROR("Failed to open status file")
        struct stat status_file_attributes;
        if (fstat(status_file_fd, &status_file_attributes) == -1)
            HANDLE_ERROR("Failed to query status file attributes")
        if (sendfile(connected_socket_fd, status_file_fd, 0, status_file_attributes.st_size) == -1)
            HANDLE_ERROR("Failed to send status file")
        if (close(status_file_fd) == -1)
            HANDLE_ERROR("Failed to close status file")
        sleep(1); // Some browsers on the client side seem to need this, otherwise they say the connection was reset
        if (close(connected_socket_fd) == -1)
            HANDLE_ERROR("Failed to close connected socket")
        LOG("Served client")
    }

    // Ctrl-C default behaviour exits the program, causing the kernel to close the sockets automatically
}
