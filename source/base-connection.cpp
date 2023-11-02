//
// Created by brian on 10/12/23.
//

#include "cpool/base-connection.hpp"

namespace xhl {
	base::base(conn_config& conf) {
		_aliveTime = clock();
	}

	void base::refreshAliveTime() {
		_aliveTime = clock();
	}

	clock_t base::getAliveTime() const {
		return clock() - _aliveTime;
	}
} // xhl