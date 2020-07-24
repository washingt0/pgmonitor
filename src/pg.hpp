#include <pqxx/connection>

#ifndef PGMONITOR_PG_HPP
#define PGMONITOR_PG_HPP

struct pid_info {
    int
        pid,
        client_port;
    std::string
        datname,
        usename,
        application_name,
        client_addr,
        since,
        query,
        state,
        since_state;
};

class Conn {
    pqxx::connection *conn;
    const char *conn_str;

public:
    explicit Conn(const char *conn_str);
    pid_info* get_pid_info(int *pid);
};

#endif //PGMONITOR_PG_HPP