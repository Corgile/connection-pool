#include "cpool/connection-pool.hpp"
#include "sqlite_conn.hpp"



int main() {
	xhl::conn_config cf{
			.url = "./temp.db",
			.tb = "students",
			.max_idle = 5,
			.timeout_sec = 10,
	};
	xhl::pool_config pf = {
			.init_size = 3,
			.max_size = 5
	};
	xhl::config cnf(cf, pf);

	auto pool = xhl::connection_pool<xhl::sqlite_connection>::create(cnf);
	auto conn = pool->get_connection();
	student s1{.name = "Alice", .age=20, .grade=95.5};
	student s2{.name = "Bob", .age=22, .grade=88.0};
	student s3{.name = "Charlie", .age=21, .grade=75.5};
	conn->insert(s1);
	conn->insert(s2);
	conn->insert(s3);
	conn->retrieveStudents();
	return 0;
}
