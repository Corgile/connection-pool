//
// Created by brian on 10/12/23.
//

#include "cpool/base-connection.hpp"

namespace xhl {
	base_connection::base_connection(conn_config& conf) {
		_aliveTime = clock();
	}

	void base_connection::refreshAliveTime() {
		_aliveTime = clock();
	}

	clock_t base_connection::getAliveTime() const {
		return clock() - _aliveTime;
	}
} // xhl