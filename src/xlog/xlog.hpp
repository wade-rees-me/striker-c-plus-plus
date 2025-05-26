#ifndef XLOG_HPP
#define XLOG_HPP

#include <cstdarg>
#include <cstdint>
#include <string>
#include <time.h>

namespace xlog {

constexpr const char *SYSLOG_ADDRESS = "192.168.0.27";
constexpr uint16_t SYSLOG_PORT = 10514;
constexpr size_t SYSLOG_MSG_MAX = 1024;
constexpr int SYSLOG_FACILITY = (1 << 3);

enum SyslogSeverity {
    SYSLOG_EMERG = 0,
    SYSLOG_ALERT = 1,
    SYSLOG_CRIT = 2,
    SYSLOG_ERR = 3,
    SYSLOG_WARNING = 4,
    SYSLOG_NOTICE = 5,
    SYSLOG_INFO = 6,
    SYSLOG_DEBUG = 7
};

void init(const std::string &remote_host = SYSLOG_ADDRESS, uint16_t port = SYSLOG_PORT);
time_t start(const char *fmt, ...);
void info(const char *fmt, ...);
void error(const char *fmt, ...);
void fatal(const char *fmt, ...);
void panic(const char *fmt, ...);
void stop(time_t start, const char *fmt, ...);
void close();

} // namespace xlog

#endif // XLOG_HPP

