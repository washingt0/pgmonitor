#include "pg.hpp"
#include <pqxx/pqxx>
#include <stdexcept>

Conn::Conn(const char *conn_str) {
    this->conn_str = conn_str;

    if (strlen(conn_str) == 0) {
        throw std::invalid_argument("conn str is empty");
    }

    this->conn = new pqxx::connection(this->conn_str);

    if (!this->conn->is_open()) {
        throw std::runtime_error("conn is dead");
    }
}

pid_info * Conn::get_pid_info(int *pid) {
    pqxx::work W{*this->conn};
    pqxx::result R{W.exec_params(
            "SELECT "
            "pid, datname, usename, application_name, client_addr, client_port, backend_start, query, state, state_change "
            "FROM pg_stat_activity "
            "WHERE backend_type = 'client backend'"
            "  AND pid = $1",
            *pid
        )
    };

    std::vector<pid_info*> data = std::vector<pid_info*>(R.size());

    pid_info *d;

    for (int i = 0; i < R.affected_rows(); i++) {
        d = new pid_info{
            R[i].at(0).as<int>(),
            R[i].at(5).as<int>(),
            R[i].at(1).as<std::string>(),
            R[i].at(2).as<std::string>(),
            R[i].at(3).as<std::string>(),
            R[i].at(4).as<std::string>(),
            R[i].at(6).as<std::string>(),
            R[i].at(7).as<std::string>(),
            R[i].at(8).as<std::string>(),
            R[i].at(9).as<std::string>(),
        };

    }

    return d;
}