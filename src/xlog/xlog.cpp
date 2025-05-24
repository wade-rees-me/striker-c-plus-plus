#include "xlog.hpp"
#include "../constants/constants.hpp"
#include <arpa/inet.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace xlog {

static int syslog_socket = -1;
static struct sockaddr_in syslog_server;
static char hostname[64] = "xlog";

void init(const std::string &remote_host, uint16_t port) {
    syslog_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (syslog_socket < 0) {
        perror("socket");
        return;
    }

    memset(&syslog_server, 0, sizeof(syslog_server));
    syslog_server.sin_family = AF_INET;
    syslog_server.sin_port = htons(port);

    if (inet_pton(AF_INET, remote_host.c_str(), &syslog_server.sin_addr) <= 0) {
        perror("inet_pton");
        return;
    }

    gethostname(hostname, sizeof(hostname));
}

void send_str(SyslogSeverity level, const char *message) {
    if (syslog_socket < 0)
        return;

    char packet[SYSLOG_MSG_MAX];
    int prefix_len = snprintf(packet, sizeof(packet), "<%d>%s: [version=%s] [PID=%d] | ", level,
                              STRIKER_WHO_AM_I.c_str(), STRIKER_VERSION.c_str(), getpid());

    if (prefix_len < 0 || prefix_len >= (int)sizeof(packet)) {
        return; // prefix formatting error
    }

    snprintf(packet + prefix_len, sizeof(packet) - prefix_len, "%s", message);

    sendto(syslog_socket, packet, strlen(packet), 0, reinterpret_cast<struct sockaddr *>(&syslog_server),
           sizeof(syslog_server));
}

void send(SyslogSeverity level, const char *fmt, va_list args) {
    if (syslog_socket < 0)
        return;

    char message[SYSLOG_MSG_MAX];
    // char packet[SYSLOG_MSG_MAX];

    vsnprintf(message, sizeof(message), fmt, args);
    send_str(level, message);

    /*
    int prefix_len = snprintf(packet, sizeof(packet), "<%d>%s: [version=%s] [PID=%d] | ", level,
                              STRIKER_WHO_AM_I.c_str(), STRIKER_VERSION.c_str(), getpid());

    if (prefix_len < 0 || prefix_len >= (int)sizeof(packet)) {
        return; // prefix formatting error
    }

    snprintf(packet + prefix_len, sizeof(packet) - prefix_len, "%s", message);

    sendto(syslog_socket, packet, strlen(packet), 0, reinterpret_cast<struct sockaddr *>(&syslog_server),
           sizeof(syslog_server));
           */
}

time_t start(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    send(SyslogSeverity::SYSLOG_INFO, fmt, args);
    va_end(args);
    return time(NULL);
}

void info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    send(SyslogSeverity::SYSLOG_INFO, fmt, args);
    va_end(args);
}

void error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    send(SyslogSeverity::SYSLOG_ERR, fmt, args);
    va_end(args);
}

void fatal(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    send(SyslogSeverity::SYSLOG_CRIT, fmt, args);
    va_end(args);
}

void panic(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    send(SyslogSeverity::SYSLOG_CRIT, fmt, args);
    va_end(args);
    exit(0);
}

void stop(time_t start_time, const char *fmt, ...) {
    time_t elapsed = time(nullptr) - start_time;
    if (elapsed < 0) {
        elapsed = 0;
    }

    // Format the user-provided message
    char user_msg[512];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(user_msg, sizeof(user_msg), fmt, args);
    va_end(args);

    // Format the final message with elapsed time
    char final_msg[1024];
    std::snprintf(final_msg, sizeof(final_msg), "%s | total time = %lds", user_msg, elapsed);

    // Send the log
    send_str(SYSLOG_INFO, final_msg);
}

void close() {
    if (syslog_socket >= 0) {
        ::close(syslog_socket);
        syslog_socket = -1;
    }
}

} // namespace xlog

